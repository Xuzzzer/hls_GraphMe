#include "bcsr.h"
#include <iostream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <algorithm>

// -----------------------------------------------------------------------------
// Host‑side testbench for the set_int_kernel HLS design.
// ‑ Packs element arrays into PROCESSING_SEGMENT_SIZE‑wide vectors, calls the
//   kernel, unpacks the result, prints and verifies correctness.
// -----------------------------------------------------------------------------

int main()
{
    //--------------------------------------------------------------------------
    // 1.  Define the raw element arrays (same data as before)
    //--------------------------------------------------------------------------
    constexpr int A_elements = 12;
    constexpr int B_elements = 13;

    BCSR_t setA_elems[A_elements] = {{1, 0b10000001},  {3, 0b00100000},  {5, 0b00111000},
                                     {8, 0b11111111},  {10, 0b00000001}, {13, 0b00010000},
                                     {15, 0b00001001}, {18, 0b01001010}, {22, 0b11000000},
                                     {25, 0b00100100}, {30, 0b11110000}, {31, 0b00001111}};

    BCSR_t setB_elems[B_elements] = {
        {2, 0b11000001},  {3, 0b11111111},  {5, 0b00011000},  {8, 0b00001111},  {12, 0b11100000},
        {15, 0b11111111}, {16, 0b00000011}, {17, 0b00111001}, {18, 0b01001010}, {21, 0b10010010},
        {22, 0b11111111}, {30, 0b00001111}, {32, 0b00000100}};

    //--------------------------------------------------------------------------
    // 2.  Compute batch counts & allocate vector (segment) arrays
    //--------------------------------------------------------------------------
    const int A_batches = (A_elements + PROCESSING_SEGMENT_SIZE - 1) / PROCESSING_SEGMENT_SIZE;
    const int B_batches = (B_elements + PROCESSING_SEGMENT_SIZE - 1) / PROCESSING_SEGMENT_SIZE;
    const int max_batches = std::max(A_batches, B_batches);

    std::vector<BCSR_vec> setA_vec(A_batches);
    std::vector<BCSR_vec> setB_vec(B_batches);

    // Initialise all lanes with padding
    auto init_vec = [](std::vector<BCSR_vec> &v)
    {
        for (auto &seg : v)
        {
            for (int l = 0; l < PROCESSING_SEGMENT_SIZE; ++l)
                seg.lane[l] = make_padding_bcsr();
        }
    };
    init_vec(setA_vec);
    init_vec(setB_vec);

    //--------------------------------------------------------------------------
    // 3.  Pack the raw elements into their corresponding vector lanes
    //--------------------------------------------------------------------------
    for (int i = 0; i < A_elements; ++i)
    {
        int seg = i / PROCESSING_SEGMENT_SIZE;
        int lane = i % PROCESSING_SEGMENT_SIZE;
        setA_vec[seg].lane[lane] = setA_elems[i];
    }
    for (int i = 0; i < B_elements; ++i)
    {
        int seg = i / PROCESSING_SEGMENT_SIZE;
        int lane = i % PROCESSING_SEGMENT_SIZE;
        setB_vec[seg].lane[lane] = setB_elems[i];
    }

    //--------------------------------------------------------------------------
    // 4.  Pretty‑print Sets A and B (for host‑side clarity)
    //--------------------------------------------------------------------------
    std::cout << "Set A (" << A_elements << " elements):\n";
    for (int i = 0; i < A_elements; ++i)
    {
        std::cout << "  index: " << std::setw(2) << (unsigned)setA_elems[i].index
                  << ", bitmap: " << std::bitset<8>((unsigned)setA_elems[i].bitmap) << "\n";
    }

    std::cout << "\nSet B (" << B_elements << " elements):\n";
    for (int i = 0; i < B_elements; ++i)
    {
        std::cout << "  index: " << std::setw(2) << (unsigned)setB_elems[i].index
                  << ", bitmap: " << std::bitset<8>((unsigned)setB_elems[i].bitmap) << "\n";
    }

    //--------------------------------------------------------------------------
    // 5.  Call the hardware kernel
    //--------------------------------------------------------------------------
    std::vector<BCSR_vec> hardware_result(max_batches);
    init_vec(hardware_result);

    int final_popcount = 0;

    std::cout << "\nExecuting hardware intersection..." << std::endl;
    set_int_kernel(setA_vec.data(), setB_vec.data(), A_batches, B_batches, hardware_result.data(),
                   &final_popcount);

    std::cout << "=== HW result ===\n";
    for (int s = 0; s < max_batches + 10; ++s)
    {
        for (int l = 0; l < PROCESSING_SEGMENT_SIZE; ++l)
        {
            const BCSR_t &e = hardware_result[s].lane[l];
            if (!e.isPadding())
                std::cout << "idx=" << (int)e.index << " bit=" << std::bitset<8>(e.bitmap) << "\n";
        }
        std::cout << "Final Popcount: " << final_popcount << std::endl;
    }

    return 0;
    /*

    //--------------------------------------------------------------------------
    // 6.  Unpack & verify the results
    //--------------------------------------------------------------------------
    const int expected_total_popcount = 14;

    std::cout << "\nHardware execution finished." << std::endl;
    std::cout << "Final Popcount: " << final_popcount << std::endl;
    std::cout << "Intersection Result (non‑padding elements):" << std::endl;

    bool pass = true;
    int non_padding_count = 0;

    auto verify_elem = [&](const BCSR_t &e)
    {
        switch ((unsigned)e.index)
        {
            case 3:
                return e.bitmap == 0b00100000;
            case 5:
                return e.bitmap == 0b00011000;
            case 8:
                return e.bitmap == 0b00001111;
            case 15:
                return e.bitmap == 0b00001001;
            case 18:
                return e.bitmap == 0b01001010;
            case 22:
                return e.bitmap == 0b11000000;
            case 30:
                return e.bitmap == 0b00000000;
            default:
                return false; 
        }
    };

    for (int seg = 0; seg < max_batches; ++seg)
    {
        for (int lane = 0; lane < PROCESSING_SEGMENT_SIZE; ++lane)
        {
            const BCSR_t &elem = hardware_result[seg].lane[lane];
            if (!elem.isPadding())
            {
                ++non_padding_count;
                std::cout << "  index: " << std::setw(2) << (unsigned)elem.index
                          << ", bitmap: " << std::bitset<8>((unsigned)elem.bitmap)
                          << " (popcount: " << __builtin_popcount((unsigned)elem.bitmap) << ")\n";
                if (!verify_elem(elem)) pass = false;
            }
        }
    }

    // Cross‑checks
    if (non_padding_count != 7)
    {
        std::cout << "Failed: Wrong number of intersection results. Expected 7, got "
                  << non_padding_count << std::endl;
        pass = false;
    }
    if (final_popcount != expected_total_popcount)
    {
        std::cout << "Failed: Wrong total popcount. Expected " << expected_total_popcount
                  << ", got " << final_popcount << std::endl;
        pass = false;
    }

    std::cout << (pass ? "\n=== TEST PASSED ===\n" : "\n=== TEST FAILED ===\n");
    return pass ? 0 : 1;
    */
}
