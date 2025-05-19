#include <cstdlib>
#include <iostream>

#include "match_3cf.h"

void test_match_3cf()
{
#pragma HLS INTERFACE m_axi port = vertex_offsets depth = V + 1 bundle = gmem0
#pragma HLS INTERFACE m_axi port = edge_list depth = E bundle = gmem1
#pragma HLS INTERFACE m_axi port = results depth = 1 bundle = gmem2

    MatchResult *results = new MatchResult();
    const int V = 3; // 顶点数
    const int E = 6; // 边数

    vertex_t *vertex_offsets = new vertex_t[V + 1];
    vertex_t *edge_list = new vertex_t[E];

    // 初始化顶点偏移数组
    vertex_offsets[0] = 0;
    vertex_offsets[1] = 2;
    vertex_offsets[2] = 4;
    vertex_offsets[3] = 6;

    // 初始化边列表
    edge_list[0] = 1;
    edge_list[1] = 2; // 顶点0的邻居
    edge_list[2] = 0;
    edge_list[3] = 2; // 顶点1的邻居
    edge_list[4] = 0;
    edge_list[5] = 1; // 顶点2的邻居

    // 初始化结果
    results->count = 0;

    match_3cf_kernel(vertex_offsets, edge_list, 3, 6, results);

    std::cout << "Found triangles: " << results->count << std::endl;
    for (int i = 0; i < results->count; i++)
    {
        std::cout << "Triangle " << i << ": (" << results->matches[i][0] << ","
                  << results->matches[i][1] << "," << results->matches[i][2] << ")" << std::endl;
    }

    // 清理内存
    delete[] vertex_offsets;
    delete[] edge_list;
    delete results;
}

int main(int argc, char *argv)
{
    try
    {
        test_match_3cf();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
