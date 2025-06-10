#include "bcsr.h"

//-----------------------------stage_min---------------------------------------------

template <int LEVEL>
static void single_cas_stage(const BcsrEle_t in_buf[PROCESSING_SEGMENT_SIZE],
                             BcsrEle_t out_buf[PROCESSING_SEGMENT_SIZE])
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION variable = in_buf complete
#pragma HLS ARRAY_PARTITION variable = out_buf complete
    const int stride = 1 << LEVEL;
    const int num_seg = PROCESSING_SEGMENT_SIZE / (2 * stride);

CAS_SEG_LOOP:
    for (int seg = 0; seg < num_seg; ++seg)
    {
#pragma HLS UNROLL
    CAS_STAGE_STRIDE_LOOP:
        for (int j = 0; j < stride; ++j)
        {
#pragma HLS UNROLL
            const int lIdx = seg * 2 * stride + j;
            const int rIdx = lIdx + stride;

            BcsrEle_t l = in_buf[lIdx];
            BcsrEle_t r = in_buf[rIdx];

            bool l_is_pad = l.bcsr.isPadding();
            bool r_is_pad = r.bcsr.isPadding();

            bool pick_L = r_is_pad || (!l_is_pad && (l.bcsr.index < r.bcsr.index ||
                                                     (l.bcsr.index == r.bcsr.index && l.fromA)));
            BcsrEle_t l_new = pick_L ? l : r;
            BcsrEle_t r_new = pick_L ? r : l;

            bool matched = (!l_new.bcsr.isPadding() && !r_new.bcsr.isPadding() &&
                            l_new.bcsr.index == r_new.bcsr.index);
            l_new.matched = matched;
            r_new.matched = matched;

            out_buf[lIdx] = l_new;
            out_buf[rIdx] = r_new;
        }
    }
}

template <int DEPTH = 2>
void min_pipeline_stage(hls::stream<BCSR_vec_last> &segA_in, hls::stream<BCSR_vec_last> &segB_in,
                        hls::stream<BcseEle_vec_last> &stream_out, hls::stream<int> total_popcount)
{
#pragma HLS DATAFLOW

    static BCSR_t buffer_a[DEPTH][PROCESSING_SEGMENT_SIZE];
    static BCSR_t buffer_b[DEPTH][PROCESSING_SEGMENT_SIZE];
    static flag consA[DEPTH][PROCESSING_SEGMENT_SIZE];
    static flag consB[DEPTH][PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = buffer_a complete dim = 2
#pragma HLS ARRAY_PARTITION variable = buffer_b complete dim = 2
#pragma HLS ARRAY_PARTITION variable = consA complete dim = 1
#pragma HLS ARRAY_PARTITION variable = consB complete dim = 1

    bool all_ready_A = true, all_ready_B = true;

    static bool a_stream_ended = false;
    static bool b_stream_ended = false;

    if (!a_stream_ended || !b_stream_ended)
    { //有一个结束的话 还得继续读，直到两个都结束
        if (all_ready_A)
        {
            BCSR_vec_last vecA = segA_in.read();
            a_stream_ended = vecA.last;
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
            { //write
#pragma HLS UNROLL
                if (consA[0][i]) { buffer_a[0][i] = vecA.data.lane[i]; }
                else if (consA[1][i]) { buffer_a[1][i] = vecA.data.lane[i]; }
            }
        }
        if (all_ready_B)
        {
            BCSR_vec_last vecB = segB_in.read();
            b_stream_ended = vecB.last;
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
            { //write
#pragma HLS UNROLL
                if (consB[0][i])
                {
                    buffer_b[0][i] = vecB.data.lane[i];
                    consB[0][i] = 0;
                }
                else
                {
                    buffer_b[1][i] = vecB.data.lane[i];
                    consB[1][i] = 0;
                }
            }
        }
    }

    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    { //判断此时是否能读
#pragma HLS UNROLL
        all_ready_A &= consA[i];
        all_ready_B &= consB[i];
    }

    //min_core
    bool chooseA[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = chooseA complete dim = 1
    BcsrEle_vec_last out1_vec_last;

compare_and_output_loop:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS UNROLL
        int j = PROCESSING_SEGMENT_SIZE - 1 - i;

        const BCSR_t &ele_a = buffer_a[0][i];
        const BCSR_t &ele_b = buffer_b[0][j];
        bool minA = ele_b.isPadding() || (!ele_a.isPadding() && ele_a.index <= ele_b.index);

        BcsrEle_t out_ele;
        out1_vec_last.data.lane[i].bcsr = minA ? ele_a : ele_b;
        out1_vec_last.data.lane[i].fromA = minA;
        out1_vec_last.data.lane[i].matched = false;
        chooseA[i] = minA;
        out1_vec_last.data.lane[i] = out_ele;
    }

    out1_vec_last.last = a_stream_ended && b_stream_ended;

buffer_update_loop:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS UNROLL
        int j = PROCESSING_SEGMENT_SIZE - 1 - i;
        if (chooseA[i])
        {
            if (!consA[1][i])
            {
                consA[0][i] = consA[1][i];
                consA[1][i] = 1;
            }
            else { consA[0][i] = 1; }
        }
        else
        {
            if (!consB[1][i])
            {
                consB[0][i] = consB[1][i];
                consB[1][i] = 1;
            }
            else { consB[0][i] = 1; }
        }
    }

    //stage2
    BcsrEle_vec temp_vec[LOG2_PRCESSING_SEGMENT_SIZE - 1];
#pragma HLS ARRAY_PARTITION variable = temp_vec
    BcsrEle_vec_last out2_vec_last;
#pragma HLS ARRAY_PARTITION variable = out2_vec_last

    out2_vec_last.last = out1_vec_last.last;

    single_cas_stage<LOG2_PRCESSING_SEGMENT_SIZE - 1>(out1_vec_last.data, temp_vec[0]);
    single_cas_stage<LOG2_PRCESSING_SEGMENT_SIZE - 2>(temp_vec[0], temp_vec[1]);
    single_cas_stage<0>(temp_vec[LOG2_PRCESSING_SEGMENT_SIZE - 2], out2_vec_last.data);

    //stage3
    static BcsrEle_t last_reg;
#pragma HLS RESET variable = last_reg
    BcsrEle_vec_last out3_vec_last;
#pragma HLS ARRAY_PARTITION variable = out3_vec_last

    out2_vec_last.last = out2_vec_last.last;

BITMAP_PROCESS_LOOP:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS UNROLL
        const BcsrEle_t &l = (i == 0) ? last_reg : out2_vec_last.data.lane[i - 1];
        const BcsrEle_t &r = out2_vec_last.data.lane[i];
        BcsrEle_t &out_elem = out3_vec_last.data.lane[i];
        BcsrEle_t out_elem = l;

        bool matched = !l.bcsr.isPadding() && !r.bcsr.isPadding() && (l.bcsr.index == r.bcsr.index);

        if (op_is_intersection)
        {
            out_elem.bcsr.bitmap = matched ? (l.bcsr.bitmap & r.bcsr.bitmap) : bcsr_bitmap_t(0);
        }
        else
        { // Set Difference (A-B)
            if (matched) { out_elem.bcsr.bitmap = l.bcsr.bitmap & (~r.bcsr.bitmap); }
            else
            {
                out_elem.bcsr.bitmap =
                    (l.fromA && !l.bcsr.isPadding()) ? l.bcsr.bitmap : bcsr_bitmap_t(0);
            }
        }

        if (l.bcsr.isPadding()) { out_elem.bcsr = make_padding_bcsr(); }
    }
    last_reg = out2_vec_last.data.lane[PROCESSING_SEGMENT_SIZE - 1];

    //stage4
    BcsrEle_vec_last out4_vec_last;
#pragma HLS ARRAY_PARTITION variable = out4_vec_last

    out4_vec_last.last = out3_vec_last.last;

    int wr = 0;
    int pc_sum = 0;
    int total_popcount;

COMPACT_SCAN_LOOP:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
    {
#pragma HLS UNROLL
        const BcsrEle_t &e = out3_vec_last.data.lane[i];
        bool keep = (e.bcsr.bitmap != 0);
        if (keep)
        {
            out4_vec_last.data.lane[wr++] = e.bcsr;
            pc_sum += e.bcsr.popcount();
        }
    }

PADDING_FILL_LOOP:
    for (int k = wr; k < PROCESSING_SEGMENT_SIZE; ++k)
    {
#pragma HLS UNROLL
        out4_vec_last.data.lane[k] = make_padding_bcsr(); // bitmap=0 Padding->width
    }

    total_popcount = pc_sum;

    out_stream.write(out4_vec_last);
}
