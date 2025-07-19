/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

// C++ 标准库
#include <algorithm>
#include <bitset>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// XRT 核心库
#include "experimental/xrt_xclbin.h"
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

// 项目头文件
#include "./src/bcsr.h"

// 用于分配足够大的输出缓冲区
constexpr int MAX_OUTPUT_FACTOR = 10;

/**
 * @brief 主函数，用于驱动FPGA执行集合交集运算
 */
int main(int argc, char **argv)
{
    // --- 1. 解析命令行参数 ---
    if (argc != 2)
    {
        std::cout << "用法: " << argv[0] << " <xclbin_file>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string binaryFile = argv[1];
    int device_index = 0;

    std::cout << "===== 步骤 1: 环境设置 =====" << std::endl;
    std::cout << "将在设备 " << device_index << " 上加载 XCLBIN 文件: " << binaryFile << std::endl;

    // --- 2. 初始化 FPGA 设备和 Kernel ---
    auto device = xrt::device(device_index);
    auto uuid = device.load_xclbin(binaryFile);
    auto kernel =
        xrt::kernel(device, uuid, "set_int_kernel", xrt::kernel::cu_access_mode::exclusive);

    std::cout << "设备和 Kernel 初始化完成。" << std::endl;

    // --- 3. 准备输入数据 (Host 端) ---
    std::cout << "\n===== 步骤 2: 准备输入数据 =====" << std::endl;

   
    BCSR_t setA_elems[] = {BCSR_t(1, 0b10000001),  BCSR_t(3, 0b00100000),  BCSR_t(5, 0b00111000),
                           BCSR_t(8, 0b11111111),  BCSR_t(12, 0b00000001), BCSR_t(13, 0b00010000),
                           BCSR_t(15, 0b00001001), BCSR_t(18, 0b01001011), BCSR_t(23, 0b11000000),
                           BCSR_t(25, 0b00100100), BCSR_t(29, 0b11110000), BCSR_t(30, 0b00001111),
                           BCSR_t(39, 0b00001111),BCSR_t(59, 0b11110000),BCSR_t(69, 0b11110000),BCSR_t(79, 0b11110000)};
   /* BCSR_t setB_elems[] = {BCSR_t(1, 0b10000001),  BCSR_t(3, 0b00100000),  BCSR_t(5, 0b00111000),
                           BCSR_t(8, 0b11111111),  BCSR_t(12, 0b00000001), BCSR_t(13, 0b00010000),
                           BCSR_t(15, 0b00001001), BCSR_t(18, 0b01001011), BCSR_t(23, 0b11000000),
                           BCSR_t(25, 0b00100100), BCSR_t(29, 0b11110000), BCSR_t(30, 0b00001111),
                           BCSR_t(39, 0b00001111),BCSR_t(59, 0b11110000),BCSR_t(69, 0b11110000),BCSR_t(79, 0b11110000)};*/

    BCSR_t setB_elems[] = {BCSR_t(2, 0b11000001),  BCSR_t(3, 0b11111111),  BCSR_t(5, 0b00011000),
                           BCSR_t(8, 0b00001111),  BCSR_t(12, 0b11100001), BCSR_t(15, 0b11111111),
                           BCSR_t(16, 0b00000011), BCSR_t(17, 0b00111001), BCSR_t(18, 0b01001010),
                           BCSR_t(21, 0b10010010), BCSR_t(22, 0b11111111), BCSR_t(31, 0b11111111),
                           BCSR_t(39, 0b00000100)};
      


    const int A_elements = sizeof(setA_elems) / sizeof(BCSR_t);
    const int B_elements = sizeof(setB_elems) / sizeof(BCSR_t);

    // 根据 PROCESSING_SEGMENT_SIZE 计算批次数
    const int A_batches = (A_elements + PROCESSING_SEGMENT_SIZE - 1) / PROCESSING_SEGMENT_SIZE;
    const int B_batches = (B_elements + PROCESSING_SEGMENT_SIZE - 1) / PROCESSING_SEGMENT_SIZE;
    const int max_input_batches = std::max(A_batches, B_batches);
    const int max_output_cycles = max_input_batches * MAX_OUTPUT_FACTOR; // 为输出结果分配足够的空间

    std::cout << " v: " << PROCESSING_SEGMENT_SIZE << std::endl;
    std::cout << "集合 A: " << A_elements << " 个元素, 分成 " << A_batches << " 批" << std::endl;
    std::cout << "集合 B: " << B_elements << " 个元素, 分成 " << B_batches << " 批" << std::endl;
    std::cout << "为输出缓冲区分配 " << max_output_cycles << " 个周期的空间" << std::endl;

    // 创建 Host 端的向量用于存放分批和填充后的数据
    std::vector<BCSR_vec> setA_vec(A_batches);
    std::vector<BCSR_vec> setB_vec(B_batches);

    // 初始化函数：将向量的所有 lane 填充为 padding 标记
    auto init_vec_with_padding = [](std::vector<BCSR_vec> &v)
    {
        for (auto &seg : v)
        {
            for (int l = 0; l < PROCESSING_SEGMENT_SIZE; ++l)
            {
                seg.lane[l] = make_padding_bcsr();
            }
        }
    };

    init_vec_with_padding(setA_vec);
    init_vec_with_padding(setB_vec);

    // 将原始数据打包进向量
    for (int i = 0; i < A_elements; ++i)
    {
        setA_vec[i / PROCESSING_SEGMENT_SIZE].lane[i % PROCESSING_SEGMENT_SIZE] = setA_elems[i];
    }
    for (int i = 0; i < B_elements; ++i)
    {
        setB_vec[i / PROCESSING_SEGMENT_SIZE].lane[i % PROCESSING_SEGMENT_SIZE] = setB_elems[i];
    }

    std::cout << "数据打包和填充完成。" << std::endl;

    // --- 4. 分配设备缓冲区 (Device Buffers) ---
    std::cout << "\n===== 步骤 3: 分配并同步设备缓冲区 =====" << std::endl;

    std::cout << "=== Debug ===" << std::endl;
    std::cout << "setA_vec.size() = " << setA_vec.size() << std::endl;
    std::cout << "setB_vec.size() = " << setB_vec.size() << std::endl;
    std::cout << "sizeof(BCSR_vec) = " << sizeof(BCSR_vec) << std::endl;
    std::cout << "max_output_cycles = " << max_output_cycles << std::endl;
    std::cout << "Total boSetA size = " << setA_vec.size() * sizeof(BCSR_vec) << std::endl;
    std::cout << "Total boSetB size = " << setB_vec.size() * sizeof(BCSR_vec) << std::endl;
    std::cout << "Total boResult size = " << max_output_cycles * sizeof(BCSR_vec) << std::endl;

    auto boSetA = xrt::bo(device, setA_vec.size() * sizeof(BCSR_vec), kernel.group_id(0));
    auto boSetB = xrt::bo(device, setB_vec.size() * sizeof(BCSR_vec), kernel.group_id(1));
    auto boResult = xrt::bo(device, max_output_cycles * sizeof(BCSR_vec), kernel.group_id(0));
    auto boPopcount = xrt::bo(device, max_output_cycles*sizeof(int), kernel.group_id(1));
  

    // --- 5. 将数据从 Host 拷贝到 Device ---
   
    boSetA.write(setA_vec.data());
    boSetB.write(setB_vec.data());

    auto sync_start = std::chrono::high_resolution_clock::now();
    boSetA.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    boSetB.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    auto sync_end = std::chrono::high_resolution_clock::now();

    std::cout << "输入数据已同步到设备。" << std::endl;

    // --- 6. 执行 Kernel ---
    std::cout << "\n===== 步骤 4: 执行 Kernel =====" << std::endl;
    auto run_start = std::chrono::high_resolution_clock::now();
    std::cout << "开始执行了" << std::endl;
    // 注意：将批次数 A_batches 和 B_batches 作为标量参数传递给 Kernel
    auto run = kernel(boSetA, boSetB, A_batches,B_batches,boResult, boPopcount);
    run.wait();
    auto run_end = std::chrono::high_resolution_clock::now();
    std::cout << "Kernel 执行完毕。" << std::endl;

    // --- 7. 将结果从 Device 拷贝回 Host ---
    std::cout << "\n===== 步骤 5: 获取并分析结果 =====" << std::endl;
    std::vector<BCSR_vec> hardware_result_vec(max_output_cycles);
    std::vector<int> final_popcount_vec(max_output_cycles);
    int final_popcount = 0;

    boResult.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    boPopcount.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    boResult.read(hardware_result_vec.data());
    boPopcount.read(final_popcount_vec.data());

    std::cout << "结果已从设备同步回 Host。" << std::endl;

    // --- 8. 验证并显示结果 ---
    int calculated_popcount = 0;
    int total_valid_elements = 0;

    std::cout << "\n--- 交集结果分析 ---" << std::endl;
    for (int cycle = 0; cycle < max_output_cycles; ++cycle)
    {
        bool has_valid_data_in_cycle = false;
        for (int l = 0; l < PROCESSING_SEGMENT_SIZE; ++l)
        {
            const BCSR_t &e = hardware_result_vec[cycle].lane[l];
            if (!e.isPadding())
            {
                has_valid_data_in_cycle = true;
                break;
            }
        }

        if (has_valid_data_in_cycle)
        {
            std::cout << "周期 " << std::setw(2) << cycle << " 的有效结果:" << std::endl;
            for (int l = 0; l < PROCESSING_SEGMENT_SIZE; ++l)
            {
                const BCSR_t &e = hardware_result_vec[cycle].lane[l];
                if (!e.isPadding())
                {
                    total_valid_elements++;
                    int popcount = __builtin_popcount(e.bitmap);
                    calculated_popcount += popcount;
                    std::cout << "  Lane " << std::setw(2) << l << ": index=" << std::setw(2)
                              << (int)e.index << ", bitmap=" << std::bitset<8>(e.bitmap)
                              << ", popcnt=" << popcount << std::endl;
                }
            }
        }
    }

    // --- 9. 最终验证和性能报告 ---
    std::cout << "\n--- 最终结果汇总 ---" << std::endl;

    int kernel_total_popcount = 0;
    for(int i = 0; i < max_output_cycles; ++i) {
    // 可以加一个判断，只累加有效周期的 popcount
    kernel_total_popcount += final_popcount_vec[i];
    }

    std::cout << "Kernel 报告的总 Popcount: " << kernel_total_popcount  << std::endl;
    std::cout << "Host 计算的总 Popcount:   " << calculated_popcount << std::endl;
    std::cout << "总计有效的交集元素数量: " << total_valid_elements << std::endl;

    bool pass = (kernel_total_popcount  == calculated_popcount);

    std::cout << "\n--- 性能报告 ---" << std::endl;
    std::chrono::duration<double> sync_time = sync_end - sync_start;
    std::chrono::duration<double> kernel_time = run_end - run_start;
    std::cout << "输入数据同步时间: " << sync_time.count() * 1000 << " ms" << std::endl;
    std::cout << "Kernel 执行时间: " << kernel_time.count() * 1000 << " ms" << std::endl;

    std::cout << "\nTEST " << (pass ? "✅ PASSED" : "❌ FAILED") << std::endl;

    return pass ? 0 : 1;
}