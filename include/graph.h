// graph.h
#ifndef GRAPH_H
#define GRAPH_H
#include <cstddef>
#include <string>
#include <vector>
#include "types.h"

// 定义本地缓冲区大小
#define MAX_LOCAL_BUFFER 4096

#ifdef __cplusplus
extern "C"
{
#endif

    void gpm_kernel(vertex_t *vertex_, vertex_t *edge_, int v_cnt, int e_cnt, int *pattern_matrix,
                    int pattern_size, int dim, PatternType pattern_type, MatchResult *results);

#ifdef __cplusplus
}
#endif

class Graph
{
  public:
    int v_cnt;         // 顶点数
    int e_cnt;         // 边数
    vertex_t *vertex_; // CSR: offset 数组
    vertex_t *edge_;   // CSR: 边数组

    // 构造函数和析构函数
    explicit Graph(vertex_t v_count, edge_t e_count)
        : v_cnt(v_count), e_cnt(e_count), vertex_(nullptr), edge_(nullptr)
    {
    }
    virtual ~Graph()
    {
        delete[] vertex_;
        delete[] edge_;
    }
    // 禁止拷贝
    Graph(const Graph &) = delete;
    Graph &operator=(const Graph &) = delete;
};

struct Pattern
{
    int vertex_count;
    int edge_count;
    std::vector<std::pair<int, int>> edges; // (src, dst) pairs
};

// gpm
void triangle_match(vertex_t *vertex_, vertex_t *edge_, int v_cnt, int e_cnt, MatchResult *results);

void rectangle_match(vertex_t *vertex_, vertex_t *edge_, int v_cnt, int e_cnt,
                     MatchResult *results);
void get_neighbors(vertex_t *vertex_, vertex_t *edge_, vertex_t id, vertex_t *neighbors,
                   int &num_neighbors);

#endif
