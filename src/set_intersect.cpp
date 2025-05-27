#include "../include/set_intersect.h"

void set_intersection(vertex_t *l1, int size1, vertex_t *l2, int size2, vertex_t *result,
                      int &result_size)
{
#pragma HLS INLINE
    int i = 0, j = 0, k = 0;
COMPARE_LOOP:
    while (i < size1 && j < size2)
    {
#pragma HLS PIPELINE II = 1
        if (l1[i] < l2[j]) { i++; }
        else if (l2[j] < l1[i]) { j++; }
        else
        {
            result[k] = l1[i];
            i++;
            j++;
            k++;
        }
    }
    result_size = k;
}
