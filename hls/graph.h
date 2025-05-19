// match_3cf.h
#ifndef MATCH_3CF_H
#define MATCH_3CF_H
#include <cstddef>

#include "types.h"

// 定义本地缓冲区大小
#define MAX_LOCAL_BUFFER 1024

// 3CF匹配内核函数声明
extern "C"
{
    void match_3cf_kernel(vertex_t* vertex_, vertex_t* edge_, int v_cnt, int e_cnt,
                          MatchResult* results);
}

#endif  // MATCH_3CF_H
