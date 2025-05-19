#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// 自定义头文件
#include "../include/types.h"
#include "../src/graph.h"
#include "../src/graph_loader.h"  // 引入 GraphLoader 和 Graph 类
#include "experimental/xrt_kernel.h"
#include "xrt/xrt.h"

// OpenCL 仿真相关
namespace xcl
{
std::vector<unsigned char> read_binary_file(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return std::vector<unsigned char>();
    }

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();
    return buffer;
}

std::vector<std::string> get_xil_devices() { return {"xilinx_device"}; }
}  // namespace xcl

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "用法: " << argv[0] << " <图文件> [sim]" << std::endl;
        return 1;
    }

    std::string graph_file = argv[1];
    bool simulation_mode = (argc > 2 && strcmp(argv[2], "sim") == 0);

    std::cout << "加载图文件: " << graph_file << std::endl;

    try
    {
        // 使用 GraphLoader 加载图
        GraphLoader loader;
        auto graph = loader.load_from_edge_list(graph_file);

        std::cout << "加载完成: " << graph->v_cnt << " 个顶点, " << graph->e_cnt << " 条边"
                  << std::endl;

        // 软件仿真模式下运行 count_triangles()
        if (simulation_mode)
        {
            std::cout << "运行软件仿真模式..." << std::endl;

            long long triangle_count = graph->count_triangles();

            std::cout << "找到 " << triangle_count << " 个三角形。" << std::endl;
        }
        else
        {
            std::cout << "运行 FPGA 硬件加速模式..." << std::endl;

            // 1. 加载 XCLBIN 文件
            auto binary = xcl::read_binary_file("match_3cf_kernel.xclbin");
            if (binary.empty())
            {
                std::cerr << "无法加载 xclbin 文件" << std::endl;
                return -1;
            }

            // 2. 初始化设备和上下文
            auto device = xrt::device(xcl::get_xil_devices()[0]);
            auto uuid = device.load_xclbin(binary);
            auto krnl = xrt::kernel(device, uuid, "match_3cf_kernel");

            // 3. 分配共享内存缓冲区
            size_t vertex_size = (graph->v_cnt + 1) * sizeof(vertex_t);
            size_t edge_size = graph->e_cnt * sizeof(vertex_t);
            size_t result_size = sizeof(MatchResult);

            xrt::bo bo_vertex(device, vertex_size, krnl.group_id(0));
            xrt::bo bo_edge(device, edge_size, krnl.group_id(1));
            xrt::bo bo_result(device, result_size, krnl.group_id(4));

            // 4. 将数据写入缓冲区
            bo_vertex.write(graph->vertex_);
            bo_edge.write(graph->edge_);
            MatchResult results{};
            bo_result.write(&results);

            // 5. 同步缓冲区到设备
            bo_vertex.sync(XCL_BO_SYNC_BO_TO_DEVICE);
            bo_edge.sync(XCL_BO_SYNC_BO_TO_DEVICE);
            bo_result.sync(XCL_BO_SYNC_BO_TO_DEVICE);

            // 6. 运行内核
            auto run = krnl(bo_vertex, bo_edge, graph->v_cnt, graph->e_cnt, bo_result);
            run.wait();

            // 7. 同步结果回主机
            bo_result.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

            // 8. 读取结果
            bo_result.read(&results);
            std::cout << "FPGA 找到 " << results.count << " 个三角形。" << std::endl;

            // 可选：打印部分匹配结果
            for (int i = 0; i < std::min(results.count, 10); ++i)
            {
                std::cout << "(" << results.matches[i][0] << ", " << results.matches[i][1] << ", "
                          << results.matches[i][2] << ")" << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "错误: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
