/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/
#include "../include/bcsr.h"
#include <chrono>
//#include "cmdlineparser.h"
#include <iostream>
#include <cstring>

// XRT includes
#include "xrt/xrt_bo.h"
#include <experimental/xrt_xclbin.h>
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

#define DATA_SIZE 1024000

int main(int argc, char **argv)
{

    std::cout << "argc = " << argc << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }

    // Read settings
    std::string binaryFile = "./vadd.xclbin";
    int device_index = 0;

    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin("./vadd.xclbin");
    // Create kernel
    auto kernel =
        xrt::kernel(device, uuid, "set_int_kernel", xrt::kernel::cu_access_mode::exclusive);
    // Test data setup
    const int A_elements = 12;
    const int B_elements = 13;

    // Initialize input data
    BCSR_t setA[A_elements] = {{1, 0b10000001},  {3, 0b00100000},  {5, 0b00111000},
                               {8, 0b11111111},  {10, 0b00000001}, {13, 0b00010000},
                               {15, 0b00001001}, {18, 0b01001010}, {22, 0b11000000},
                               {25, 0b00100100}, {30, 0b11110000}, {31, 0b00001111}};

    BCSR_t setB[B_elements] = {
        {2, 0b11000001},  {3, 0b11111111},  {5, 0b00011000},  {8, 0b00001111},  {12, 0b11100000},
        {15, 0b11111111}, {16, 0b00000011}, {17, 0b00111001}, {18, 0b01001010}, {21, 0b10010010},
        {22, 0b11111111}, {30, 0b00001111}, {32, 0b00000100}};

    // Allocate device buffers
    auto boSetA = xrt::bo(device, A_elements * sizeof(BCSR_t), kernel.group_id(0));
    auto boSetB = xrt::bo(device, B_elements * sizeof(BCSR_t), kernel.group_id(1));
    auto boResult = xrt::bo(device, PROCESSING_SEGMENT_SIZE * sizeof(BCSR_t), kernel.group_id(2));
    auto boPopcount = xrt::bo(device, sizeof(int), kernel.group_id(3));

    // Copy input data to device buffers
    boSetA.write(setA);
    boSetB.write(setB);

    std::cout << "Synchronizing input buffers..." << std::endl;
    auto sync_start = std::chrono::high_resolution_clock::now();
    boSetA.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    boSetB.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    auto sync_end = std::chrono::high_resolution_clock::now();

    std::cout << "Executing kernel..." << std::endl;
    auto run_start = std::chrono::high_resolution_clock::now();
    auto run = kernel(boSetA, boSetB, boResult, boPopcount, A_elements, B_elements);
    run.wait();
    auto run_end = std::chrono::high_resolution_clock::now();

    // Get results
    std::cout << "Reading results..." << std::endl;
    BCSR_t hardware_result[PROCESSING_SEGMENT_SIZE];
    int final_popcount;

    boResult.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    boPopcount.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    boResult.read(hardware_result);
    boPopcount.read(&final_popcount);

    // Print timing information
    std::chrono::duration<double> sync_time = sync_end - sync_start;
    std::chrono::duration<double> kernel_time = run_end - run_start;

    std::cout << "\nTiming Results:" << std::endl;
    std::cout << "Input sync time: " << sync_time.count() << " seconds" << std::endl;
    std::cout << "Kernel execution time: " << kernel_time.count() << " seconds" << std::endl;

    // Verify results
    const int expected_total_popcount = 14;
    bool pass = true;
    int non_padding_count = 0;

    std::cout << "\nIntersection Results:" << std::endl;
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
        if (!hardware_result[i].isPadding())
        {
            non_padding_count++;
            std::cout << "  index: " << std::setw(2) << hardware_result[i].index
                      << ", bitmap: " << std::bitset<8>(hardware_result[i].bitmap) << std::endl;
        }
    }

    if (final_popcount != expected_total_popcount)
    {
        std::cout << "ERROR: Wrong popcount! Expected " << expected_total_popcount << ", got "
                  << final_popcount << std::endl;
        pass = false;
    }

    std::cout << "\nTEST " << (pass ? "PASSED" : "FAILED") << std::endl;
    return pass ? 0 : 1;
}