#include <cstdlib>
#include <iostream>

#include "../include/graph.h"
#include "../include/types.h"
#include "graph_loader.h"

void test_graph_pattern_matching(const std::string &dataset, const std::string &pattern_file)
{
    auto graph = GraphLoader::load_from_edge_list(dataset);
    if (!graph) { throw std::runtime_error("Failed to load graph from " + dataset); }

    int pattern_v_cnt = 0;
    int pattern_e_cnt = 0;

    PatternType pattern_type;
    auto pattern =
        GraphLoader::load_pattern_matrix(pattern_file, pattern_v_cnt, pattern_e_cnt, pattern_type);
    if (pattern.empty()) { throw std::runtime_error("Pattern is empty!"); }

    MatchResult *results = new MatchResult();
    if (!results) { throw std::runtime_error("Failed to allocate results memory"); }

    results->count = 0;

    gpm_kernel(graph->vertex_, graph->edge_, graph->v_cnt, graph->e_cnt, pattern[0].data(),
               pattern_v_cnt, pattern_e_cnt, pattern_type, results);

    std::cout << "Found matches: " << results->count << std::endl;

    delete results;
}

int main(int argc, char **argv)
{
    try
    {
        /*
        if (argc != 3)
        {
            std::cerr << "Usage: " << argv[0] << " <graph_file> <pattern_matrix>" << std::endl;
            return 1;
        }
        */
        const std::string graph_file =
            "/data-ssd/home/zihanxu/hls_GraphMe/dataset/wiki-vote_input.txt";
        const std::string pattern_file = "/data-ssd/home/zihanxu/hls_GraphMe/dataset/rectangle.txt";
        test_graph_pattern_matching(graph_file, pattern_file);
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
