#ifndef TYPES_H
#define TYPES_H

#pragma once
#include <ap_int.h>
#include <hls_stream.h>

#ifdef __SYNTHESIS__
#include <ap_int.h>
#include <hls_stream.h>
typedef ap_uint<32> vertex_t;
typedef ap_uint<32> edge_t;
typedef ap_uint<32> offset_t;
#else // Host端（测试、仿真、加载等）
#include <cstdint>
typedef uint32_t vertex_t;
typedef uint32_t edge_t;
typedef uint32_t offset_t;
#endif

#define MAX_VERTICES 10000
#define MAX_EDGES 300000
#define MAX_MATCHES 60000000
#define MAX_PATTERN_VERTICES 16

static const int W = 32; // BCSR word
static const int B = 8;  // bitmap bits

struct MatchResult
{
    int count;
    //vertex_t matches[MAX_MATCHES][4];
};

struct BCSRGraph
{
    vertex_t v_cnt;
    edge_t e_cnt;
    offset_t vertex_[MAX_VERTICES + 1];
    vertex_t edge_[MAX_EDGES];
};

// Pattern的邻接矩阵表示
struct PatternMatrix
{
    int vertex_count;
    int edge_count;
    int matrix[MAX_PATTERN_VERTICES][MAX_PATTERN_VERTICES];
};

enum class PatternType
{
    TRIANGLE,
    RECTANGLE
};

#endif // TYPES_H
