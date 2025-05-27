// graph.cpp
#include "../include/graph.h"
#include "../include/set_intersect.h"

// #include <hls_stream.h>

// 3CF
void triangle_match(vertex_t *vertex_, vertex_t *edge_, int v_cnt, int e_cnt, MatchResult *results)
{

    vertex_t neighbors_v1[MAX_LOCAL_BUFFER];
    vertex_t neighbors_v2[MAX_LOCAL_BUFFER];
    vertex_t intersection_result[MAX_LOCAL_BUFFER];

#pragma HLS RESOURCE variable = neighbors_v1 core = RAM_1P_BRAM
#pragma HLS RESOURCE variable = neighbors_v2 core = RAM_1P_BRAM
#pragma HLS RESOURCE variable = intersection_result core = RAM_1P_BRAM

    if (results != NULL) { results->count = 0; }
    else
        return;

    if (v_cnt <= 0 || v_cnt > MAX_VERTICES) { return; }

    for (vertex_t v1 = 0; v1 < v_cnt; v1++)
    {
        int num_neighbors_v1;
        get_neighbors(vertex_, edge_, v1, neighbors_v1, num_neighbors_v1);

        for (int i = 0; i < num_neighbors_v1; i++)
        {
            vertex_t v2 = neighbors_v1[i];
            // v3>v2>v1
            if (v2 <= v1) continue;
            int num_neighbors_v2;
            get_neighbors(vertex_, edge_, v2, neighbors_v2, num_neighbors_v2);

            int intersection_size;
            set_intersection(neighbors_v1, num_neighbors_v1, neighbors_v2, num_neighbors_v2,
                             intersection_result, intersection_size);

            for (int j = 0; j < intersection_size; j++)
            {
                vertex_t v3 = intersection_result[j];
                if (v3 <= v2) continue;
                if (results->count < MAX_MATCHES)
                {
                    //results->matches[results->count][0] = v1;
                    //results->matches[results->count][1] = v2;
                    //results->matches[results->count][2] = v3;
                    results->count++;
                }
            }
        }
    }
}

/*
1-2
| |
4-3
*/

void rectangle_match(vertex_t *vertex_, vertex_t *edge_, int v_cnt, int e_cnt, MatchResult *results)
{

    vertex_t neighbors_v1[MAX_LOCAL_BUFFER];
    vertex_t neighbors_v2[MAX_LOCAL_BUFFER];
    vertex_t neighbors_v3[MAX_LOCAL_BUFFER];
    vertex_t intersection_result[MAX_LOCAL_BUFFER];

#pragma HLS RESOURCE variable = neighbors_v1 core = RAM_1P_BRAM
#pragma HLS RESOURCE variable = neighbors_v2 core = RAM_1P_BRAM
#pragma HLS RESOURCE variable = neighbors_v3 core = RAM_1P_BRAM
#pragma HLS RESOURCE variable = intersection_result core = RAM_1P_BRAM

    if (results != NULL) { results->count = 0; }
    else { return; }

    if (v_cnt <= 0 || v_cnt > MAX_VERTICES) { return; }
    for (vertex_t v1 = 0; v1 < v_cnt; ++v1)
    {
        int deg1;
        get_neighbors(vertex_, edge_, v1, neighbors_v1, deg1);

        //  v2 > v1
        for (int i = 0; i < deg1; ++i)
        {
            vertex_t v2 = neighbors_v1[i];
            if (v2 <= v1) continue;

            int deg2;
            get_neighbors(vertex_, edge_, v2, neighbors_v2, deg2);

            for (int j = 0; j < deg2; ++j)
            {
                vertex_t v3 = neighbors_v2[j];

                // v1 < v3
                if (v3 <= v1) continue;
                //if (bool_in_neighbors(v3, neighbors_v1, deg1)) continue;

                int deg3;
                get_neighbors(vertex_, edge_, v3, neighbors_v3, deg3);

                int isize;
                set_intersection(neighbors_v1, deg1, neighbors_v3, deg3, intersection_result,
                                 isize);

                for (int k = 0; k < isize; ++k)
                {
                    vertex_t v4 = intersection_result[k];

                    // v2 < v4
                    if (v4 <= v2) continue;

                    if (results->count < MAX_MATCHES) { ++results->count; }
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
    if (num_neighbors > MAX_LOCAL_BUFFER)
    {
        printf("get_neighblos > MAX_LOCAL_BUFFER");
        num_neighbors = MAX_LOCAL_BUFFER;
    }

    for (int i = 0; i < num_neighbors; i++)
    {
#pragma HLS PIPELINE II = 1
        neighbors[i] = edge_[start + i];
    }
}
