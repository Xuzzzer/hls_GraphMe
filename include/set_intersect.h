#ifndef SET_INTERSECT_H
#define SET_INTERSECT_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // 集合交集函数声明
    void set_intersection(vertex_t *l1, int size1, vertex_t *l2, int size2, vertex_t *result,
                          int &result_size);

#ifdef __cplusplus
}
#endif

#endif // SET_INTERSECT_H