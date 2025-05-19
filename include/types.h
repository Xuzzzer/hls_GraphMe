#ifndef TYPES_H
#define TYPES_H

#pragma once
#include <ap_int.h>
#include <hls_stream.h>

typedef ap_uint<32> vertex_t;
typedef ap_uint<32> edge_t;
typedef ap_uint<32> offset_t;

#define MAX_VERTICES 10000
#define MAX_EDGES 300000
#define MAX_MATCHES 10000

struct MatchResult
{
    int count;
    vertex_t matches[MAX_MATCHES][3];
};

struct CSRGraph
{
    vertex_t v_cnt;
    edge_t e_cnt;
    offset_t vertex_[MAX_VERTICES + 1];  // CSR offset array
    vertex_t edge_[MAX_EDGES];           // CSR edge array
};

#endif  // TYPES_H
