我确认过了，第一个脚本就有输出，第二个脚本就没有输出“ #include "bcsr.h"
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

    int
    main()
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
”第二个“ #include "bcsr.h"
#include <iostream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <algorithm>
    int
    main()
{
    // 定义原始元素数组
    constexpr int A_elements = 8;
    constexpr int B_elements = 8;
    BCSR_t setA_elems[A_elements] = {{1, 0b10000001},  {3, 0b00100000},  {5, 0b00111000},
                                     {8, 0b11111111},  {10, 0b00000001}, {13, 0b00010000},
                                     {15, 0b00001001}, {18, 0b01001010}};
    BCSR_t setB_elems[B_elements] = {{2, 0b11000001},  {3, 0b11111111},  {5, 0b00011000},
                                     {8, 0b00001111},  {12, 0b11100000}, {15, 0b11111111},
                                     {16, 0b00000011}, {17, 0b00111001}}; /*
    BCSR_t setA_elems[A_elements] = {{1, 0b10000001},  {3, 0b00100000},  {5, 0b00111000},
                                     {8, 0b11111111},  {10, 0b00000001}, {13, 0b00010000},
                                     {15, 0b00001001}, {18, 0b01001010}, {22, 0b11000000},
                                     {25, 0b00100100}, {30, 0b11110000}, {31, 0b00001111}};
    BCSR_t setB_elems[B_elements] = {
        {2, 0b11000001},  {3, 0b11111111},  {5, 0b00011000},  {8, 0b00001111},  {12, 0b11100000},
        {15, 0b11111111}, {16, 0b00000011}, {17, 0b00111001}, {18, 0b01001010}, {21, 0b10010010},
        {22, 0b11111111}, {30, 0b00001111}, {32, 0b00000100}};*/
    // 计算批次数量
    const int A_batches = (A_elements + PROCESSING_SEGMENT_SIZE - 1) / PROCESSING_SEGMENT_SIZE;
    const int B_batches = (B_elements + PROCESSING_SEGMENT_SIZE - 1) / PROCESSING_SEGMENT_SIZE;
    const int max_batches = std::max(A_batches, B_batches);
    std::cout << "A_batches: " << A_batches << ", B_batches: " << B_batches
              << ", max_batches: " << max_batches << std::endl;
    std::cout << "PROCESSING_SEGMENT_SIZE: " << PROCESSING_SEGMENT_SIZE << std::endl;
    std::vector<BCSR_vec> setA_vec(A_batches);
    std::vector<BCSR_vec> setB_vec(B_batches);
    // 初始化所有lanes为padding
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
    // 打包原始元素到对应的vector lanes
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
    // 打印输入数据验证
    std::cout << "\n=== Input Data Verification ===" << std::endl;
    std::cout << "Set A packed data:" << std::endl;
    for (int s = 0; s < A_batches; ++s)
    {
        std::cout << "Batch " << s << ": ";
        for (int l = 0; l < PROCESSING_SEGMENT_SIZE; ++l)
        {
            const BCSR_t &e = setA_vec[s].lane[l];
            if (!e.isPadding())
            {
                std::cout << "(" << (int)e.index << "," << std::bitset<8>(e.bitmap) << ") ";
            }
            else { std::cout << "(PAD) "; }
        }
        std::cout << std::endl;
    }
    std::cout << "Set B packed data:" << std::endl;
    for (int s = 0; s < B_batches; ++s)
    {
        std::cout << "Batch " << s << ": ";
        for (int l = 0; l < PROCESSING_SEGMENT_SIZE; ++l)
        {
            const BCSR_t &e = setB_vec[s].lane[l];
            if (!e.isPadding())
            {
                std::cout << "(" << (int)e.index << "," << std::bitset<8>(e.bitmap) << ") ";
            }
            else { std::cout << "(PAD) "; }
        }
        std::cout << std::endl;
    }
    // 预期需要的输出周期数（这是一个上界估计）
    const int max_output_cycles = max_batches * 2; // 保守估计
    std::vector<BCSR_vec> hardware_result(max_output_cycles);
    init_vec(hardware_result);
    int final_popcount = 0;
    std::cout << "\n=== Executing hardware intersection ===" << std::endl;
    set_int_kernel(setA_vec.data(), setB_vec.data(), A_batches, B_batches, hardware_result.data(),
                   &final_popcount);
    std::cout << "\n=== Hardware Results Analysis ===" << std::endl;
    std::cout << "Final Popcount: " << final_popcount << std::endl;
    // 动态确定实际输出的周期数
    int actual_output_cycles = 0;
    int total_valid_elements = 0;
    int cycle_popcount_sum = 0;
    for (int cycle = 0; cycle < max_output_cycles; ++cycle)
    {
        bool has_valid_data = false;
        int cycle_valid_count = 0;
        int cycle_popcount = 0;
        std::cout << "Cycle " << cycle << " results:" << std::endl;
        for (int l = 0; l < PROCESSING_SEGMENT_SIZE; ++l)
        {
            const BCSR_t &e = hardware_result[cycle].lane[l];
            if (!e.isPadding())
            {
                has_valid_data = true;
                cycle_valid_count++;
                int popcount = __builtin_popcount(e.bitmap);
                cycle_popcount += popcount;
                std::cout << "  Lane " << l << ": idx=" << (int)e.index
                          << " bitmap=" << std::bitset<8>(e.bitmap) << " popcount=" << popcount
                          << std::endl;
            }
        }
        if (has_valid_data)
        {
            actual_output_cycles = cycle + 1;
            total_valid_elements += cycle_valid_count;
            cycle_popcount_sum += cycle_popcount;
            std::cout << "  Cycle valid elements: " << cycle_valid_count
                      << ", Cycle popcount: " << cycle_popcount << std::endl;
        }
        else if (actual_output_cycles > 0)
        {
            // 如果之前有有效数据但现在没有，说明可能结束了
            std::cout << "  No valid data - possibly end of stream" << std::endl;
            break;
        }
        else { std::cout << "  No valid data" << std::endl; }
        std::cout << std::endl;
    }
    std::cout << "=== Summary ===" << std::endl;
    std::cout << "Actual output cycles: " << actual_output_cycles << std::endl;
    std::cout << "Total valid elements: " << total_valid_elements << std::endl;
    std::cout << "Calculated total popcount: " << cycle_popcount_sum << std::endl;
    std::cout << "Hardware reported popcount: " << final_popcount << std::endl;
    // 验证期望结果
    std::cout << "\n=== Expected Results Verification ===" << std::endl;
    std::cout << "Expected intersection results:" << std::endl;
    std::cout << "  idx=3  bitmap=00100000 (A & B: 00100000 & 11111111)" << std::endl;
    std::cout << "  idx=5  bitmap=00011000 (A & B: 00111000 & 00011000)" << std::endl;
    std::cout << "  idx=8  bitmap=00001111 (A & B: 11111111 & 00001111)" << std::endl;
    std::cout << "  idx=15 bitmap=00001001 (A & B: 00001001 & 11111111)" << std::endl;
    std::cout << "  idx=18 bitmap=01001010 (A & B: 01001010 & 01001010)" << std::endl;
    std::cout << "  idx=22 bitmap=11000000 (A & B: 11000000 & 11111111)" << std::endl;
    std::cout << "  idx=30 bitmap=00000000 (A & B: 11110000 & 00001111)" << std::endl;
    std::cout << "Expected total popcount: 1+2+4+2+3+2+0 = 14" << std::endl;
    return 0;
}
”