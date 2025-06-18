#include "graph_loader.h"
#include "../include/types.h"
#include <fstream>
#include <set>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <utility>

std::vector<std::vector<int>> GraphLoader::load_pattern_matrix(const std::string &pattern_file,
                                                               int &pattern_v_cnt,
                                                               int &pattern_e_cnt,
                                                               PatternType &pattern_type)
{
    std::ifstream fin(pattern_file);
    if (!fin) throw std::runtime_error("Failed to open pattern file: " + pattern_file);

    //pattern_type
    std::string type_str;
    if (!std::getline(fin, type_str)) throw std::runtime_error("Failed to read pattern type");
    if (type_str == "TRIANGLE")
        pattern_type = PatternType::TRIANGLE;
    else if (type_str == "RECTANGLE")
        pattern_type = PatternType::RECTANGLE;

    std::vector<std::vector<int>> pattern;
    std::string line;
    int row_index = 0;

    while (std::getline(fin, line))
    {
        std::istringstream iss(line);
        std::vector<int> row;
        int val;
        int col_index = 0;
        while (iss >> val)
        {
            row.push_back(val);
            if (val == 1 && col_index > row_index) { pattern_e_cnt++; }
            col_index++;
        }
        if (!row.empty()) { pattern.push_back(row); }
        row_index++;
    }
    pattern_v_cnt = pattern.size();

    if (pattern_v_cnt > MAX_PATTERN_VERTICES)
    {
        throw std::runtime_error("Pattern size exceeds maximum allowed vertices");
    }

    for (const auto &row : pattern)
    {
        if ((int)row.size() != pattern_v_cnt)
        {
            throw std::runtime_error("Pattern matrix is not square!");
        }
    }
    return pattern;
}

std::unique_ptr<Graph> GraphLoader::load_from_file(const std::string &filename, FileFormat format)
{
    switch (format)
    {
        case FileFormat::EDGE_LIST:
            return load_from_edge_list(filename);
        case FileFormat::ADJACENCY_LIST:
            return load_from_adj_list(filename);
        case FileFormat::MATRIX:
            return load_from_matrix(filename);
        default:
            throw std::runtime_error("Unsupported file format");
    }
}

std::unique_ptr<Graph> GraphLoader::load_from_edge_list(const std::string &filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp) { throw std::runtime_error("Cannot open file: " + filename); }

    int v_cnt;
    unsigned int e_cnt;
    if (fscanf(fp, "%d%u", &v_cnt, &e_cnt) != 2)
    {
        fclose(fp);
        throw std::runtime_error("Failed to read vertex and edge count");
    }

    // 分配边空间（双向边 * 2），其中next_id代表实际的最终的顶点个数
    std::pair<vertex_t, vertex_t> *edges = new std::pair<vertex_t, vertex_t>[e_cnt * 2];
    edge_t tmp_e = 0;
    vertex_t next_id = 0;
    std::unordered_map<vertex_t, vertex_t> id_map;

    // 分配degree数组
    vertex_t *degree = new vertex_t[v_cnt];
    memset(degree, 0, sizeof(vertex_t) * v_cnt);

    vertex_t x, y;
    while (fscanf(fp, "%d%d", &x, &y) == 2)
    {
        if (x == y)
        {
            printf("find self circle\n");
            continue;
        }
        if (!id_map.count(x)) id_map[x] = next_id++;
        if (!id_map.count(y)) id_map[y] = next_id++;
        x = id_map[x];
        y = id_map[y];
        edges[tmp_e++] = std::make_pair(x, y);
        edges[tmp_e++] = std::make_pair(y, x);
        ++degree[x];
        ++degree[y];
    }
    fclose(fp);

    // 排序 去重
    std::sort(edges, edges + tmp_e);
    auto unique_end = std::unique(edges, edges + tmp_e);
    edge_t unique_edges = unique_end - edges;

    auto graph = std::make_unique<Graph>(next_id, unique_edges);
    graph->v_cnt = next_id;
    graph->e_cnt = unique_edges;

    // 构建 offset
    vertex_t *vertex_array = new vertex_t[next_id + 1];
    vertex_t idx = 0;
    for (vertex_t i = 0; i < next_id; ++i)
    {
        vertex_array[i] = idx;
        idx += degree[i];
    }
    vertex_array[next_id] = idx;
    graph->vertex_ = vertex_array;

    // 分配并设置顶点和边数组
    vertex_t *edge_array = new vertex_t[unique_edges];
    for (edge_t i = 0; i < unique_edges; ++i)
    {
        edge_array[i] = edges[i].second;
    }
    graph->edge_ = edge_array;

    // 2或者是指针拷贝，memcpy(graph->edge_, &(edges[0].second), unique_edges *
    // sizeof(int));
    /*3
  int* edge_ = new int[unique_edges];
  std::vector<int> pos(next_id);
  //拷贝vertex_ to pos
  for (int i = 0; i < next_id; ++i) {
  pos[i] = vertex_[i];
  }
  for (i = 0; i < unique_edges; ++i) {
      int u = edges[i].first;
      int v = edges[i].second;
      edge_[pos[u]++] = v;
  }
  */

    // 清理
    delete[] edges;
    delete[] degree;

    return graph;
}

std::unique_ptr<Graph> GraphLoader::load_from_adj_list(const std::string &filename)
{
    throw std::runtime_error("Not implemented");
}

std::unique_ptr<Graph> GraphLoader::load_from_matrix(const std::string &filename)
{
    throw std::runtime_error("Not implemented");
}
