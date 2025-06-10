#include "bcsr.h"

//-----------------------------stage_min---------------------------------------------

template <int DEPTH = 2>
void min_pipeline_stage(hls::stream<BCSR_vec_last> &segA_in, hls::stream<BCSR_vec_last> &segB_in,
                        hls::stream<BcseEle_vec_last> &minstage_out)
{
#pragma HLS DATAFLOW

    static BCSR_t buffer_a[DEPTH][N];
    static BCSR_t buffer_b[DEPTH][N];
    static flag consA[DEPTH][N];
    static flag consB[DEPTH][N];
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
                if (consA[0][i])
                {
                    buffer_a[0][i] = vecA.data.lane[i];
                    consA[0][i] = 0;
                }
                else
                {
                    buffer_a[1][i] = vecA.data.lane[i];
                    consA[1][i] = 0;
                }
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

    for (int i = 0; i < N; ++i)
    { //判断此时是否能读
#pragma HLS UNROLL
        all_ready_A &= consA[i];
        all_ready_B &= consB[i];
    }

    //min_core
    bool chooseA[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = chooseA complete dim = 1
    BcsrEle_vec_last out_vec_last;

compare_and_output_loop:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS UNROLL
        int j = PROCESSING_SEGMENT_SIZE - 1 - i;

        const BCSR_t &ele_a = buffer_a[0][i];
        const BCSR_t &ele_b = buffer_b[0][j];
        bool minA = ele_b.isPadding() || (!ele_a.isPadding() && ele_a.index <= ele_b.index);

        BcsrEle_t out_ele;
        out_vec_last.data.lane[i].bcsr = minA ? ele_a : ele_b;
        out_vec_last.data.lane[i].fromA = minA;
        out_vec_last.data.lane[i].matched = false;
        chooseA[i] = minA;
        out_vec_last.data.lane[i] = out_ele;
    }

    out_vec_last.last = a_stream_ended && b_stream_ended;

    minstage_out.write(out_vec_last);

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
}

//----------------------------------------------------pipeline2-----------------------------

template <int LEVEL>
void single_cas_stage(hls::stream<BcsrEle_t> &in_stream, hls::stream<BcsrEle_t> &out_stream)
{
PROCESS_SEGMENTS_LOOP:

    BcsrEle_t in_buf[PROCESSING_SEGMENT_SIZE];
    BcsrEle_t out_buf[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = in_buf complete
#pragma HLS ARRAY_PARTITION variable = out_buf complete

READ_SEGMENT:
    for (int j = 0; j < PROCESSING_SEGMENT_SIZE; ++j)
    {
#pragma HLS PIPELINE II = 1
        in_buf[j] = in_stream.read();
    }

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

WRITE_SEGMENT:
    for (int j = 0; j < PROCESSING_SEGMENT_SIZE; ++j)
    {
#pragma HLS PIPELINE II = 1
        out_stream.write(out_buf[j]);
    }
}

void cas_pipeline_stage(hls::stream<BcsrEle_t> &in_stream, hls::stream<BcsrEle_t> &out_stream)
{

#pragma HLS DATAFLOW

    hls::stream<BcsrEle_t> streams[LOG2_PRCESSING_SEGMENT_SIZE - 1];
#pragma HLS STREAM variable = streams depth = PROCESSING_SEGMENT_SIZE

    single_cas_stage<LOG2_PRCESSING_SEGMENT_SIZE - 1>(in_stream, streams[0]);
    single_cas_stage<LOG2_PRCESSING_SEGMENT_SIZE - 2>(streams[0], streams[1]);
    single_cas_stage<0>(streams[LOG2_PRCESSING_SEGMENT_SIZE - 2], out_stream);
}

//----------------stage_bitmap---------------------------------------------

void bitmap_pipeline_stage(hls::stream<BcsrEle_t> &in_stream, hls::stream<BcsrEle_t> &out_stream,
                           const bool op_is_intersection)
{
#pragma HLS INLINE off

    BcsrEle_t in_buf[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = in_buf complete
    static BcsrEle_t last_reg;
#pragma HLS RESET variable = last_reg

READ_SEGMENT_LOOP:
    for (int j = 0; j < PROCESSING_SEGMENT_SIZE; ++j)
    {
#pragma HLS PIPELINE II = 1
        in_buf[j] = in_stream.read();
    }

BITMAP_PROCESS_LOOP:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS PIPELINE II = 1
        const BcsrEle_t &l = (i == 0) ? last_reg : in_buf[i - 1];
        const BcsrEle_t &r = in_buf[i];
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
        out_stream.write(out_elem);
    }
    last_reg = in_buf[PROCESSING_SEGMENT_SIZE - 1];
}

//---------------compact_bitmap---------------------------------------------
void compact_pipeline_stage(hls::stream<BcsrEle_t> &in_stream, hls::stream<BCSR_t> &out_stream,
                            int &total_popcount)
{
#pragma HLS INLINE off
    BcsrEle_t in_buf[PROCESSING_SEGMENT_SIZE];
    BCSR_t out_buf[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = in_buf complete
#pragma HLS ARRAY_PARTITION variable = out_buf complete

    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS PIPELINE II = 1
        in_buf[i] = in_stream.read();
    }

    int wr = 0;
    int pc_sum = 0;

COMPACT_SCAN_LOOP:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
    {
#pragma HLS UNROLL
        const BcsrEle_t &e = in_buf[i];
        bool keep = (e.bcsr.bitmap != 0);
        if (keep)
        {
            out_buf[wr++] = e.bcsr;
            pc_sum += e.bcsr.popcount();
        }
    }

PADDING_FILL_LOOP:
    for (int k = wr; k < PROCESSING_SEGMENT_SIZE; ++k)
    {
#pragma HLS UNROLL
        out_buf[k] = make_padding_bcsr(); // bitmap=0 Padding->width
    }

COMPACT_WRITE_LOOP:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS PIPELINE II = 1
        out_stream.write(out_buf[i]);
    }

    total_popcount = pc_sum;
}

//---------------top----------------------------------------------
void set_intersection_pipeline_top(hls::stream<BcsrEle_vec_last> &streamA,
                                   hls::stream<BcsrEle_vec_last> &streamB,
                                   hls::stream<BCSR_t> &stream_out, int &final_popcount)
{
#pragma HLS DATAFLOW

    hls::stream<BcsrEle_t> min_to_cas("min_to_cas_stream");
    hls::stream<BcsrEle_t> cas_to_bitmap("cas_to_bitmap_stream");
    hls::stream<BcsrEle_t> bitmap_to_compact("bitmap_to_compact_stream");

    void min_pipeline_stage(hls::stream<BCSR_vec_last> & segA_in,
                            hls::stream<BCSR_vec_last> & segB_in,
                            hls::stream<BcseEle_vec_last> & minstage_out)

        min_pipeline_stage(streamA, streamB, min_to_cas);

    cas_pipeline_stage(min_to_cas, cas_to_bitmap);

    bitmap_pipeline_stage(cas_to_bitmap, bitmap_to_compact, true);

    compact_pipeline_stage(bitmap_to_compact, stream_out, final_popcount);
}