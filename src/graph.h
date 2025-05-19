//graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>
#include <stdexcept>

class Graph {
public:
    int v_cnt;                  // 顶点数
    int e_cnt;                  // 边数
    unsigned int *vertex_;  // CSR: offset 数组
    int* edge_;    // CSR: 边数组

    // 构造函数和析构函数
    explicit Graph(int v_cnt);
    virtual ~Graph() {
        delete[] vertex_;
        delete[] edge_;
    }

    int get_vertex_count() const { return v_cnt; }
    int get_edge_count() const { return e_cnt; }

    // 数三角形
    long long count_triangles();

    int get_degree(int v) const {
        return vertex_[v+1] - vertex_[v];
    }

   unsigned int* get_vertex_array() const {return vertex_; }
   int* get_edge_array() const {return edge_;}

private:
  

};

#endif