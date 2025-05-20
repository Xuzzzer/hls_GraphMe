#ifndef GRAPH_LOADER_H
#define GRAPH_LOADER_H

#include "../include/graph.h"
#include <string>
#include <algorithm> // for sort, unique
#include <cstring>   // for memset
#include <memory>
#include <stdexcept>

enum class FileFormat
{
    EDGE_LIST,      // 每行两个数字表示一条边
    ADJACENCY_LIST, // 每行第一个数字表示节点，后面的数字表示邻居
    MATRIX          // 邻接矩阵格式
};

class GraphLoader
{
  public:
    static std::vector<std::vector<int>> load_pattern_matrix(const std::string &pattern_file,
                                                             int &pattern_v_cnt, int &pattern_e_cnt,
                                                             PatternType &pattern_type);
    //file
    static std::unique_ptr<Graph> load_from_file(const std::string &filename, FileFormat format);

    // 具体
    static std::unique_ptr<Graph> load_from_edge_list(const std::string &filename);
    static std::unique_ptr<Graph> load_from_adj_list(const std::string &filename);
    static std::unique_ptr<Graph> load_from_matrix(const std::string &filename);

  private:
    GraphLoader() = delete;
};

#endif // GRAPH_LOADER_H