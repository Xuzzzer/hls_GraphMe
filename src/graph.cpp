#include "graph.h"
#include <algorithm>
#include <unordered_set>

Graph::Graph(int v_cnt) : v_cnt(v_cnt), e_cnt(0), vertex_(nullptr), edge_(nullptr) {
    if (v_cnt < 0) {
        throw std::invalid_argument("Vertex count must be non-negative");
    }
}

long long Graph::count_triangles() {
    long long triangle_count = 0;

    for (int u = 0; u < v_cnt; ++u) {
        int begin_u = vertex_[u];
        int end_u = vertex_[u + 1];

        // 遍历u的所有邻居v，且保证v > u
        for (int i = begin_u; i < end_u; ++i) {
            int v = edge_[i];
            if (v <= u) continue;  // 只处理v > u的情况

            int begin_v = vertex_[v];
            int end_v = vertex_[v + 1];

            // 查找u和v的共同邻居w，且w > v
            int ptr1 = begin_u;
            int ptr2 = begin_v;

            while (ptr1 < end_u && ptr2 < end_v) {
                int w1 = edge_[ptr1];
                int w2 = edge_[ptr2];

                if (w1 == w2) {
                    if (w1 > v) {  // w > v > u
                        ++triangle_count;
                    }
                    ++ptr1;
                    ++ptr2;
                } else if (w1 < w2) {
                    ++ptr1;
                } else {
                    ++ptr2;
                }
            }
        }
    }

    return triangle_count;
}