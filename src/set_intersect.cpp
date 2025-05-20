#include "set_intersect.h"

void set_intersection(vertex_t *set_a, int size_a, vertex_t *set_b, int size_b, vertex_t *result,
                      int &result_size)
{

    result_size = 0;
    if (size_a <= 0 || size_b <= 0) { return; }
    int i = 0, j = 0;

#pragma HLS PIPELINE II = 1
    while (i < size_a && j < size_b)
    {
        if (set_a[i] == set_b[j])
        {
            result[result_size++] = set_a[i];
            i++;
            j++;
        }
        else if (set_a[i] < set_b[j]) { i++; }
        else { j++; }
    }
}
