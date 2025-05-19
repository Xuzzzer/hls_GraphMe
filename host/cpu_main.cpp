#include <fstream>
#include <iostream>
#include "../src/common.h"
#include "../src/graph.h"
#include "../src/graph_loader.h"

void print_usage()
{
    std::cout << "Usage: ./graphme <input_file> [format]" << std::endl;
    std::cout << "Formats:" << std::endl;
    std::cout << "  edge    - Edge list format (default)" << std::endl;
    std::cout << "  adj     - Adjacency list format" << std::endl;
    std::cout << "  matrix  - Adjacency matrix format" << std::endl;
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 2)
        {
            print_usage();
            return 1;
        }

        std::string filename = argv[1];
        FileFormat format = FileFormat::EDGE_LIST;

        if (argc >= 3)
        {
            std::string format_str = argv[2];
            if (format_str == "adj")
                format = FileFormat::ADJACENCY_LIST;
            else if (format_str == "matrix")
                format = FileFormat::MATRIX;
            else
            {
                std::cerr << "Unknown format: " << format_str << std::endl;
                print_usage();
                return 1;
            }
        }

        // 使用GraphLoader加载图数据
        auto graph = GraphLoader::load_from_file(filename, format);
        if (!graph)
        {
            std::cerr << "Failed to load graph from " << filename << std::endl;
            return 1;
        }
        // 计算三角形数量#include "common.h"

        // ...

        double start = get_wall_time();
        long long result = graph->count_triangles();
        double end = get_wall_time();

        printf("Time taken: %.6f seconds\n", end - start);

        std::cout << "Number of vertices: " << graph->get_vertex_count() << std::endl;
        std::cout << "Number of edges: " << graph->get_edge_count() << std::endl;
        std::cout << "Number of triangles: " << result << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}