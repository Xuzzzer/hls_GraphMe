#include "../include/bcsr.h"
#include <iostream>
int main()
{
    BCSR_t setA[PROCESSING_SEGMENT_SIZE];
    BCSR_t setB[PROCESSING_SEGMENT_SIZE];
    BCSR_t hardware_result[PROCESSING_SEGMENT_SIZE];
    int final_popcount = 0;
    int A_elements = 12;
    int B_elements = 13;
    int max_elements = A_elements > B_elements ?: A_elements : B_elements;

    for (int i = 0; i < max_elements; ++i)
    {
        setA[i] = make_padding_bcsr();
        setB[i] = make_padding_bcsr();
    }

    BCSR_t setA[A_elements] = {{1, 0b10000001},  {3, 0b00100000},  {5, 0b00111000},
                               {8, 0b11111111},  {10, 0b00000001}, {13, 0b00010000},
                               {15, 0b00001001}, {18, 0b01001010}, {22, 0b11000000},
                               {25, 0b00100100}, {30, 0b11110000}, {31, 0b00001111}};
    BCSR_t setB[B_elements] = {
        {2, 0b11000001},  {3, 0b11111111},  {5, 0b00011000},  {8, 0b00001111},  {12, 0b11100000},
        {15, 0b11111111}, {16, 0b00000011}, {17, 0b00111001}, {18, 0b01001010}, {21, 0b10010010},
        {22, 0b11111111}, {30, 0b00001111}, {32, 0b00000100}};

    std::cout << "Set A (" << A_elements << " elements):" << std::endl;
    for (int i = 0; i < A_elements; i++)
    {
        std::cout << "  index: " << std::setw(2) << setA[i].index
                  << ", bitmap: " << std::bitset<8>(setA[i].bitmap) << std::endl;
    }

    std::cout << "\nSet B (" << B_elements << " elements):" << std::endl;
    for (int i = 0; i < B_elements; i++)
    {
        std::cout << "  index: " << std::setw(2) << setB[i].index
                  << ", bitmap: " << std::bitset<8>(setB[i].bitmap) << std::endl;
    }

    // --- Execute the Hardware Pipeline ---
    std::cout << "\nExecuting hardware intersection..." << std::endl;
    set_int_kernel(setA, setB, hardware_result, A_elements, B_elements, &final_popcount);

    // Verification
    std::cout << "\nHardware execution finished." << std::endl;
    std::cout << "Final Popcount: " << final_popcount << std::endl;
    std::cout << "Intersection Result (non-padding elements):" << std::endl;

    bool pass = true;
    int non_padding_count = 0;

    // Expected intersection:
    // index 3: 1010 & 1111 = 1010 (popcount 2)
    // index 8: 1111 & 0011 = 0011 (popcount 2)
    // Total expected popcount = 4
    // Expected intersection results:
    // index 3: 0b00100000 & 0b11111111 = 0b00100000 (popcount 1)
    // index 5: 0b00111000 & 0b00011000 = 0b00011000 (popcount 2)
    // index 8: 0b11111111 & 0b00001111 = 0b00001111 (popcount 4)
    // index 15: 0b00001001 & 0b11111111 = 0b00001001 (popcount 2)
    // index 18: 0b01001010 & 0b01001010 = 0b01001010 (popcount 3)
    // index 22: 0b11000000 & 0b11111111 = 0b11000000 (popcount 2)
    // index 30: 0b11110000 & 0b00001111 = 0b00000000 (popcount 0)
    const int expected_total_popcount = 14;

    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
        if (!hardware_result[i].isPadding())
        {
            non_padding_count++;
            std::cout << "  index: " << std::setw(2) << hardware_result[i].index
                      << ", bitmap: " << std::bitset<8>(hardware_result[i].bitmap)
                      << " (popcount: " << __builtin_popcount(hardware_result[i].bitmap) << ")"
                      << std::endl;

            // Verify specific cases
            switch (hardware_result[i].index)
            {
                case 3:
                    if (hardware_result[i].bitmap != 0b00100000) pass = false;
                    break;
                case 5:
                    if (hardware_result[i].bitmap != 0b00011000) pass = false;
                    break;
                case 8:
                    if (hardware_result[i].bitmap != 0b00001111) pass = false;
                    break;
                case 15:
                    if (hardware_result[i].bitmap != 0b00001001) pass = false;
                    break;
                case 18:
                    if (hardware_result[i].bitmap != 0b01001010) pass = false;
                    break;
                case 22:
                    if (hardware_result[i].bitmap != 0b11000000) pass = false;
                    break;
                case 30:
                    if (hardware_result[i].bitmap != 0b00000000) pass = false;
                    break;
                default:
                    pass = false; // Unexpected index
            }
        }
    }

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

    if (pass) { std::cout << "\n=== TEST PASSED ===" << std::endl; }
    else { std::cout << "\n=== TEST FAILED ===" << std::endl; }

    return pass ? 0 : 1;
}