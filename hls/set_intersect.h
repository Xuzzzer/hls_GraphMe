#ifndef SET_INTERSECT_H
#define SET_INTERSECT_H

#include "ap_int.h"
#include "types.h"


// 定义本地缓冲区大小
#define MAX_LOCAL_BUFFER 1024

// 集合交集函数声明
void set_intersection(
    vertex_t* set_a, int size_a,
    vertex_t* set_b, int size_b,
    vertex_t* result, int& result_size
);

#endif // SET_INTERSECT_H
