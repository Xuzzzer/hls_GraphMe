// match_3cf.cpp
#include "match_3cf.h"

#include "set_intersect.h"

// #include <hls_stream.h>

#define MAX_LOCAL_BUFFER 1024

// 3CF
extern "C"
{
    void 3cf_kernel(vertex_t *vertex_, vertex_t *edge_, int v_cnt, int e_cnt, MatchResult *results)
    {
        // HLS pragmas
#pragma HLS INTERFACE m_axi port = vertex_ offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edge_ offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = results offset = slave bundle = gmem2

#pragma HLS INTERFACE s_axilite port = v_cnt bundle = control
#pragma HLS INTERFACE s_axilite port = e_cnt bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

        // 本地缓存
        vertex_t local_neighbors_a[MAX_EDGES];
        vertex_t local_neighbors_b[MAX_EDGES];
        vertex_t intersection_result[MAX_EDGES];

// 本地缓存 - 使用BRAM资源
#pragma HLS RESOURCE variable = local_neighbors_a core = RAM_1P_BRAM
#pragma HLS RESOURCE variable = local_neighbors_b core = RAM_1P_BRAM
#pragma HLS RESOURCE variable = intersection_result core = RAM_1P_BRAM

        if (results != NULL) { results->count = 0; }
        else
        {
            return; // 防止空指针
        }

        if (v_cnt <= 0 || v_cnt > MAX_VERTICES) { return; }

        for (vertex_t v1 = 0; v1 < v_cnt; v1++)
        {
            int num_neighbors_v1;
            get_neighbors(vertex_, edge_, v1, local_neighbors_a, num_neighbors_v1);

            for (int i = 0; i < num_neighbors_v1; i++)
            {
                vertex_t v2 = local_neighbors_a[i];

                // v3>v2>v1
                if (v2 <= v1) continue;
                int num_neighbors_v2;
                get_neighbors(vertex_, edge_, v2, local_neighbors_b, num_neighbors_v2);
                int intersection_size;

                set_intersection(local_neighbors_a, num_neighbors_v1, local_neighbors_b,
                                 num_neighbors_v2, intersection_result, intersection_size);

                for (int j = 0; j < intersection_size; j++)
                {
                    vertex_t v3 = intersection_result[j];
                    if (v3 <= v2) continue;
                    if (results->count < MAX_MATCHES)
                    {
                        results->matches[results->count][0] = v1;
                        results->matches[results->count][1] = v2;
                        results->matches[results->count][2] = v3;
                        results->count++;
                    }
                }
            }
        }
    }
}

void get_neighbors(vertex_t *vertex_, vertex_t *edge_, vertex_t id, vertex_t *neighbors,
                   int &num_neighbors)
{
#pragma HLS INLINE

    int start = vertex_[id];
    int end = vertex_[id + 1];
    num_neighbors = end - start;

    // 安全检查：确保不超过最大缓冲区大小 ---1024
    if (num_neighbors > MAX_LOCAL_BUFFER) { num_neighbors = MAX_LOCAL_BUFFER; }

    for (int i = 0; i < num_neighbors; i++)
    {
#pragma HLS PIPELINE II = 1
        neighbors[i] = edge_[start + i];
    }
}