#ifndef BCSR_H
#define BCSR_H

#include <ap_int.h>
#include <hls_stream.h>

const int PROCESSING_SEGMENT_SIZE = 4; // N, analogous to SMA_SIZE
const int LOG2_PRCESSING_SEGMENT_SIZE = 2;
const int BCSR_BITMAP_WIDTH = 8;
const int BCSR_INDEX_WIDTH = 24; // Assuming 32-bit total BCSR element (32 - 8 = 24)
#define DEPTH 2
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

inline bool is_all_padding(BCSR_t buffer[PROCESSING_SEGMENT_SIZE])
{
#pragma HLS INLINE
    bool all_pad = true;
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS UNROLL
        all_pad &= buffer[i].isPadding();
    }
    return all_pad;
}

void single_cas_stage(const BcsrEle_vec_last in_buf[PROCESSING_SEGMENT_SIZE],
                      BcsrEle_vec_last out_buf[PROCESSING_SEGMENT_SIZE], int level);
void bitonic_stage(hls::stream<BCSR_vec_last> &segA_in, hls::stream<BCSR_vec_last> &segB_in,
                   bool op_is_intersection, hls::stream<BCSR_vec_last> &stream_out,
                   hls::stream<int> &total_popcount);

extern "C"
{
    void set_int_kernel(BCSR_vec *setA_in, BCSR_vec *setB_in, int A_batches, int B_batches,
                        BCSR_vec *o3_stream, int *o3_popcount);
}
#endif