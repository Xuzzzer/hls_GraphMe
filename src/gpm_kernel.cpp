#include "../include/graph.h"
#include "../include/types.h"
#include "../include/set_intersect.h"

extern "C"
{
    void gpm_kernel(vertex_t *vertex_, vertex_t *edge_, int v_cnt, int e_cnt, int *pattern_matrix,
                    int pattern_v_cnt, int pattern_e_cnt, PatternType pattern_type,
                    MatchResult *results)
    {
#pragma HLS INTERFACE m_axi port = vertex_ offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edge_ offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = pattern_matrix offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = results offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = vertex_ bundle = control
#pragma HLS INTERFACE s_axilite port = edge_ bundle = control
#pragma HLS INTERFACE s_axilite port = pattern_matrix bundle = control
#pragma HLS INTERFACE s_axilite port = results bundle = control
#pragma HLS INTERFACE s_axilite port = v_cnt bundle = control
#pragma HLS INTERFACE s_axilite port = e_cnt bundle = control
#pragma HLS INTERFACE s_axilite port = pattern_v_cnt bundle = control
#pragma HLS INTERFACE s_axilite port = pattern_e_cnt bundle = control
#pragma HLS INTERFACE s_axilite port = pattern_type bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

        if (results != NULL) { results->count = 0; }
        else { return; }

        switch (pattern_type)
        {
            case PatternType::TRIANGLE:
                triangle_match(vertex_, edge_, v_cnt, e_cnt, results);
                break;
            case PatternType::RECTANGLE:
                rectangle_match(vertex_, edge_, v_cnt, e_cnt, results);
                break;
            default:
                results->count = 0;
                break;
        }
    }
}