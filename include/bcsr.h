#ifndef BCSR_H
#define BCSR_H

#include <ap_int.h>
#include <hls_stream.h>

const int PROCESSING_SEGMENT_SIZE = 8; // N, analogous to SMA_SIZE
const int LOG2_PRCESSING_SEGMENT_SIZE = 3;
const int BCSR_BITMAP_WIDTH = 8;
const int BCSR_INDEX_WIDTH = 24; // Assuming 32-bit total BCSR element (32 - 8 = 24)

using bcsr_index_t = ap_uint<BCSR_INDEX_WIDTH>;
using bcsr_bitmap_t = ap_uint<BCSR_BITMAP_WIDTH>;

typedef ap_uint<1> flag;

struct BCSR_t
{
    bcsr_index_t index;
    bcsr_bitmap_t bitmap;

    BCSR_t() : index(0), bitmap(0) {}
    BCSR_t(bcsr_index_t idx, bcsr_bitmap_t bm) : index(idx), bitmap(bm) {}

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
    BcsrEle_t() : fromA(false), matched(false) {}
};

struct BCSR_vec
{
    BCSR_t lane[PROCESSING_SEGMENT_SIZE];
};

struct BCSR_vec_last
{
    BCSR_vec data;
    bool last;
};

struct BcsrEle_vec
{
    BcsrEle_t lane[PROCESSING_SEGMENT_SIZE];
};

struct BcsrEle_vec_last
{
    BcsrEle_vec data;
    bool last;
};

inline BCSR_t make_padding_bcsr()
{
    return BCSR_t(0, 0);
}

void min_pipeline_stage(hls::stream<BCSR_t> &segA_in, hls::stream<BCSR_t> &segB_in,
                        hls::stream<BcsrEle_t> &minstage_out);
template <int LEVEL>
void single_cas_stage(hls::stream<BcsrEle_t> &in_stream, hls::stream<BcsrEle_t> &out_stream);

void cas_pipeline_stage(hls::stream<BcsrEle_t> &in_stream, hls::stream<BcsrEle_t> &out_stream);

void bitmap_pipeline_stage(hls::stream<BcsrEle_t> &in_stream, hls::stream<BcsrEle_t> &out_stream,
                           const bool op_is_intersection);

void compact_pipeline_stage(hls::stream<BcsrEle_t> &in_stream, hls::stream<BCSR_t> &out_stream,
                            int &total_popcount);

// ...existing code...

// Change the implementation to declaration only
void set_intersection_pipeline_top(hls::stream<BCSR_t> &streamA, hls::stream<BCSR_t> &streamB,
                                   hls::stream<BCSR_t> &stream_out, int &final_popcount);

#endif