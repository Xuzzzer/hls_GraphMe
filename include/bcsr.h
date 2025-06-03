#ifndef BCSR_H
#define BCSR_H

#include <ap_int.h>
#include <hls_stream.h> // 目前计划使用数组，后续使用 hls::stream

const int PROCESSING_SEGMENT_SIZE = 8; // N, analogous to SMA_SIZE
const int LOG2_PRCESSING_SEGMENT_SIZE = 3;
const int BCSR_BITMAP_WIDTH = 8;
const int BCSR_INDEX_WIDTH = 24; // Assuming 32-bit total BCSR element (32 - 8 = 24)

using bcsr_index_t = ap_uint<BCSR_INDEX_WIDTH>;
using bcsr_bitmap_t = ap_uint<BCSR_BITMAP_WIDTH>;

struct BCSR_t
{
    bcsr_index_t index;
    bcsr_bitmap_t bitmap;

    BCSR_t() : index(0), bitmap(0) {}
    BCSR_t(bcsr_index_t idx, bcsr_bitmap_t bm) : index(idx), bitmap(bm) {}

    bool operator<(const BCSR_t &other) const
    {
        if (index != other.index) { return index < other.index; }
        return bitmap < other.bitmap;
    }
    bool isPadding() const
    {
        return bitmap == 0;
    }
    bool isEmptyForCompaction() const
    {
        return bitmap == 0;
    }
    int popcount() const
    {
#pragma HLS INLINE
        int cnt = 0;
        for (int i = 0; i < BCSR_BITMAP_WIDTH; ++i)
        {
#pragma HLS UNROLL
            if (bitmap[i]) ++cnt;
        }
        return cnt;
    }
};

struct BcsrEle_t
{
    BCSR_t bcsr;
    bool fromA;
    bool matched;
    bool valid;
    BcsrEle_t() : fromA(false), matched(false), valid(true) {}
};

void bcsr_set_op(const BCSR_t segA_in[PROCESSING_SEGMENT_SIZE], int lenA,
                 const BCSR_t segB_in{PROCESSING_SEGMENT_SIZE}, int lenB, bool is_intersection_op,
                 BCSR_t final_bcsr_out[PROCESSING_SEGMENT_SIZE], int &num_valid_elements_out;
                 int &total_set_bits_out;);

#endif