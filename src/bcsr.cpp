#include "bcsr.h"

//valid index from && e1(first) < e2
static bool compare_bcsr_ele(const BcsrEle_t &e1, const BcsrEle_t &e2)
{
#pragma HLS INLINE
    if (e1.valid && !e2.valid) return true;
    if (!e1.valid && e2.valid) return false;
    if (!e1.valid && !e2.valid) return false;

    if (e1.bcsr.index < e2.bcsr.index) return true;
    if (e1.bcsr.index > e2.bcsr.index) return false;

    if (e1.fromA && !e2.fromA) return true;
    if (!e1.fromA && e2.fromA) return false;

#ifndef __SYNTHESIS__
    // 仅仿真阶段输出调试信息
    printf("Warning: compare_bcsr_ele called on two identical elements (index=%u, bitmap=%u)\n",
           (unsigned)e1.bcsr.index, (unsigned)e1.bcsr.bitmap);
#endif
    return false; // 相等时返回 false，保持稳定排序即可
}

// ---------------------- Pipeline Stage Implementations-------------------
static void min_stage_core(const BCSR_t segA_in[PROCESSING_SIZE],
                           const BCSR_t segB_in[PROCESSING_SIZE], BcsrEle_t output[PROCESSING_SIZE],
                           bool consumed_A[PROCESSING_SIZE], bool consumed_B[PROCESSING_SIZE])
{
#pragma HLS INLINE

INIT_CONSUMED_A_LOOP:
    for (int k = 0; k < PROCESSING_SIZE; ++k)
    {
#pragma HLS UNROLL
        consumed_A[k] = false;
    }

INIT_CONSUMED_B_LOOP:
    for (int k = 0; k < PROCESSING_SIZE; ++k)
    {
#pragma HLS UNROLL
        consumed_B[k] = false;
    }

MIN_STAGE_CORE_LOOP:
    for (int i = 0; i < PROCESSING_SIZE; ++i)
    {
#pragma HLS UNROLL
        int idxB = PROCESSING_SIZE - 1 - i;

        const BCSR_t a = segA_in[i];
        const BCSR_t b = segB_in[idxB];

        const bool a_pad = a.isPadding();
        const bool b_pad = b.isPadding();

        bool choose_A;
        if (a_pad && b_pad)
            choose_A = true;
        else if (a_pad)
            choose_A = false;
        else if (b_pad)
            choose_A = true;
        else
            choose_A = (a.index <= b.index);

        if (choose_A)
        {
            output[i].bcsr = a;
            output[i].fromA = true;
            output[i].matched = false;
            if (!a_pad) consumed_A[i] = true;
        }
        else
        {
            output[i].bcsr = b;
            output[i].fromA = false;
            output[i].matched = false;
            if (!b_pad) consumed_B[idxB] = true;
        }
    }
}

//-----------------------------stage_min---------------------------------------------

struct MinStageIO
{
    const BCSR_t *segA;
    const BCSR_t *segB;
    bool in_valid;
    bool flush;
    bool *in_ready;

    BcsrEle_t *out_vec;
    bool out_ready;
    bool *out_valid;
};

template <int DEPTH = 2>
static void min_pipeline_stage(MinStageIO &io)
{
#pragma HLS INLINE off
#pragma HLS DATAFLOW

    static Bcsr_t fifoA[DEPTH][PROCESSING_SEGMENT_SIZE];
    static Bcsr_t fifoB[DEPTH][PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = fifoA complete dim = 2
#pragma HLS ARRAY_PARTITION variable = fifoB complete dim = 2

    static ap_uint<1> headA = 0, headB = 0;
    static ap_uint<1> tailA = 1, tailB = 1;

    BcsrEle_t core_out_buf[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = core_out_buf complete dim = 1

    static bool out_buf_valid = false;
    if (io.flush)
    {
        headA = 0;
        tailA = 1;
        headB = 0;
        tailB = 1;
        out_buf_valid = false;
    }

    *io.out_valid = out_buf_valid;
    *io.in_ready = !out_buf_valid;

    if (io.in_valid && *io.in_ready)
    {
        bool consumed_A[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = consumed_A complete dim = 1
        bool consumed_B[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = consumed_B complete dim = 1
        min_stage_core(fifoA[headA], fifoB[headB], core_out_buf, consumed_A, consumed_B);

    UPDATE_FIFO_A_LANES_LOOP:
        for (int lane = 0; lane < PROCESSING_SIZE; ++lane)
        {
#pragma HLS UNROLL
            BCSR_t current_head_A = fifoA[headA][lane];
            BCSR_t current_buff_A = fifoA[tailA][lane];
            BCSR_t new_external_A = new_segA_in[lane];

            if (consumed_A[lane])
            {
                fifoA[tailA][lane] = current_buff_A;
                fifoA[headA][lane] = new_external_A;
            }
            else
            {
                fifoA[tailA][lane] = current_head_A; //next cycle.
                fifoA[headA][lane] = current_buff_A;
                // new_external_A
            }
        }

    UPDATE_FIFO_B_LANES_LOOP:
        for (int lane = 0; lane < PROCESSING_SIZE; ++lane)
        {
#pragma HLS UNROLL
            BCSR_t current_head_B = fifoB[headB][lane];
            BCSR_t current_buff_B = fifoB[tailB][lane];
            BCSR_t new_external_B = new_segB_in[lane];
            if (consumed_B[lane])
            {
                fifoB[tailB][lane] = current_buff_B;
                fifoB[headB][lane] = new_external_B;
            }
            else
            {
                fifoB[tailB][lane] = current_head_B;
                fifoB[headB][lane] = current_buff_B;
            }
        }
        headA ^= 1;
        headB ^= 1;
        tailA ^= 1;
        tailB ^= 1;

    MIN_STAGE_OUTPUT_LOOP:
        for (int i = 0; i < PROCESSING_SIZE; ++i)
        {
#pragma HLS UNROLL
            min_out[i] = core_out[i];
        }
        *io.out_buf_valid = true;
    }
};

//// --------------CAS Stage (Single)---------------

template <int LEVEL>
static void cas_stage_core(const BcsrEle_t in_vec[PROCESSING_SEGMENT_SIZE],
                           const BcsrEle_t out_vec[PROCESSING_SEGMENT_SIZE])
{
#pragma HLS INLINE
    const int stride = 1 << LEVEL;
    const int num_seg = PROCESSING_SEGMENT_SIZE / (2 * stride);

CAS_SEG_LOOP:
    for (int seg = 0; seg < num_seg; ++seg) //seg
    {
#pragma HLS UNROLL
    CAS_STAGE_STRIDE_LOOP:
        for (int j = 0; j < stride; ++j)
        {
#pragma HLS UNROLL
            const int lIdx = seg * 2 * stride + j;
            const int rIdx = lIdx + stride;

            BcsrEle_t l = in_vec[lIdx];
            BcsrEle_t r = in_vec[rIdx];

            bool l_is_pad = l.bcsr.isPadding();
            bool r_is_pad = r.bcsr.isPadding();

            bool pick_L = r_is_pad || (!l_is_pad && (l.bcsr.index < r.bcsr.index ||
                                                     (l.bcsr.index == r.bcsr.index && l.fromA)));

            BcsrEle_t l_new = pick_L ? l : r;
            BcsrEle_t r_new = pick_L ? r : l;

            //matched
            bool matched = (!l_new.bcsr.isPadding() && !r_new.bcsr.isPadding() &&
                            l_new.bcsr.index == r_new.bcsr.index);
            l_new.matched = matched;
            r_new.matched = matched;

            out_vec[lIdx] = l_new;
            out_vec[rIdx] = r_new;
        }
    }
}

template <int LEVEL>
static void cas_single_stage(const BcsrEle_t in_vec[PROCESSING_SEGMENT_SIZE], bool in_valid,
                             bool *in_ready,

                             BcsrEle_t out_vec[PROCESSING_SEGMENT_SIZE], bool out_ready,
                             bool *out_valid, bool flush)
{
#pragma HLS INLINE off

    static BcsrEle_t buf[PROCESSING_SEGMENT_SIZE];
    static bool buf_valid = false;
#pragma HLS ARRAY_PARTITION variable = buf complete
#pragma HLS reset variable = buf_valid off /* 不跟全局复位；靠 flush */

    if (flush)
    {
        buf_valid = false;
        buf = BcsrEle_t();
    }

    *out_valid = buf_valid;
    *in_ready = (!buf_valid) || out_ready; //本级满但下一级接

    if (buf_valid && out_ready) { buf_valid = false; }

    if (in_valid && *in_ready)
    {
        BcsrEle_t tmp[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = tmp complete

        cas_stage_core<LEVEL>(in_vec, tmp);

    CAS_SINGLE_LEVEL_LOOP:
        for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
        {
#pragma HLS UNROLL
            buf[i] = tmp[i];
        }
        buf_valid = true;
    }

CAS_SINGLE_OUT_LOOP:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS UNROLL
        out_vec[i] = buf[i];
    }
}

struct CasStageIO
{
    const BcsrEle_t *in_vec;
    bool in_valid;
    bool flush;
    bool *in_ready;

    BcsrEle_t *out_vec;
    bool out_ready;
    bool *out_valid;
};

static void cas_pipeline_stage(CasStageIO &io)
{
#pragma HLS DATAFLOW
#pragma HLS ARRAY_PARTITION variable = in_vec complete
#pragma HLS ARRAY_PARTITION variable = out_vec complete

    BcsrEle_t s_vec[LOG2_PRCESSING_SEGMENT_SIZE + 1][PROCESSING_SEGMENT_SIZE];
    bool s_valid[LOG2_PRCESSING_SEGMENT_SIZE + 1];
    bool s_ready[LOG2_PRCESSING_SEGMENT_SIZE + 1];
#pragma HLS ARRAY_PARTITION variable = s_vec complete dim = 2

    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
#pragma HLS UNROLL
        s_vec[0][i] = io.in_vec[i];

    s_valid[0] = io.in_valid;
    *io.in_ready = s_ready[0];

    cas_single_stage<0>(s_vec[0], s_valid[0], &s_ready[0], s_vec[1], io.out_ready, &s_valid[1],
                        ioflush);

GEN_STAGE:
    for (int level = 1; level < LOG2_PRCESSING_SEGMENT_SIZE; ++level)
    {
#pragma HLS UNROLL
        cas_single_stage<level>(s_vec[level], s_valid[level], &s_ready[levle], s_vec[level + 1],
                                io.out_ready, &s_val[level + 1], io.flush);
    }
    s_ready[LOG2_PRCESSING_SEGMENT_SIZE] = io.out_ready;

    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
#pragma HLS UNROLL
        io.out_vec[i] = s_vec[LOG2_PRCESSING_SEGMENT_SIZE][i];

    *io.out_valid = s_valid[LOG2_PRCESSING_SEGMENT_SIZE];
}

//----------------stage_bitmap---------------------------------------------
struct BitmapStageIO
{
    const BcsrEle_t *in_vec;
    bool in_valid;
    bool opInt;
    bool flush;
    bool out_ready; //to down

    BcsrEle_t *out_vec;
    bool *out_fin;   //last data
    bool *out_valid; //out_vec_is_valid
    bool *in_ready;  //to up
};

static void bitmap_pipeline_stage(BitmapStageIO &io)
{
#pragma HLS INLINE off

    static BcsrEle_t last_reg;
    static bool fin_reg = false; //已经遇到结束符
    static bool finSent_reg = false;
    static bool valid_reg = false; //本批结果有效

#pragma HLS RESET variable = last_reg
#pragma HLS RESET variable = fin_reg
#pragma HLS RESET variable = finSent_reg
#pragma HLS RESET variable = valid_reg

    if (io.flush)
    {
        last_reg = BcsrEle_t(); // 默认padding
        fin_reg = false;
        finSent_reg = false;
        valid_reg = false;
    }

    *io.in_ready = ;
    *io.out_valid = (!finSent_reg) && valid_reg;
    *io.out_fin = fin_reg;

    //送出去数据
    if (io.out_ready && *io.out_valid)
    {
        valid_reg = false;
        finSent_reg = fin_reg;
    }

    //开始处理
    if (io.in_valid && *io.in_ready)
    {
        valid_reg = true;
    BITMAP_PROCESS_LOOP:
        for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
        {
#pragma HLS UNROLL

            const BcsrEle_t &l = (i == 0) ? last_reg : io.in_vec[i - 1];
            const BcsrEle_t &r = io.in_vec[i];
            BcsrEle_t out_elem = l;

            bool matched;
            if (i == 0)
            {
                matched =
                    (!l.bcsr.isPadding() && !r.bvsr.ispadding() && (l.bcsr.index == r.bcsr.index));
            }
            else
                matched = l.fromA && l.matched;

            //fin_reg
            if (r.fromA && r.bcsr.isEmpty()) fin_reg = true; //youwenti!!!!!!!!!!!

            if (io.opInt) { out_elem.bcsr.bitmap = matched ? (l.bcsr.bitmap & r.bcsr.bitmap) : 0; }
            else
            {
                if (matched) { out_elem.bcsr.bitmap = l.bcsr.bitmap & (~r.bcsr.bitmap); }
                else
                {
                    out_elem.bcsr.bitmap = (l.fromA && !l.bcsr.isPadding() ? l.bcsr.bitmap : 0;)
                }
            }

            if (l.bcsr.isPadding())
            {
                out_elem.bcsr.index = BCSR_PADDING_INDEX;
                out_elem.bcsr.bitmap = 0;
            }

            io.out_vec[i] = out_elem;
        }
        last_reg = io.in_vec[PROCESSING_SIZE - 1];
    }
}
//---------------compact_bitmap---------------------------------------------
static void compact_pipeline_stage(const BcsrEle_t in_vec[PROCESSING_SEGMENT_SIZE],
                                   BCSR_t out_vec[PROCESSING_SEGMENT_SIZE], int &out_len,
                                   int &out_popcount)
{
#pragma HLS INLINE off
    int wr = 0;
    int pc_sum = 0;

COMPACT_SCAN_LOOP:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
    {
#pragma HLS UNROLL
        const BcsrEle_t &e = in_vec[i];
        bool keep = (!e.bcsr.isPadding()) && (e.bcsr.bitmap != 0);
        if (keep)
        {
            out_vec[wr++] = e.bcsr;
            pc_sum += e.bcsr.popcount;
        }
    }

PADDING_FILL_LOOP:
    for (int k = wr; k < PROCESSING_SEGMENT_SIZE; ++k)
    {
#pragma HLS UNROLL
        out_vec[k] = BCSR_t(); // bitmap=0 Padding->width
    }

    out_len = wr;
    out_popcount = pc_sum;
}

//---------------top----------------------------------------------

void bcsr_set_op(const BCSR_t segA_in[PROCESSING_SEGMENT_SIZE],
                 const BCSR_t segB_in[PROCESSING_SEGMENT_SIZE], bool enable, bool flush, bool opInt,
                 //output
                 BCSR_t out_compact[PROCESSING_SEGMENT_SIZE], int &out_len, int &out_popcount,
                 bool &out_fin)
{
#pragma HLS INLINE off
#pragma HLS DATAFLOW

    BcsrEle_t min_out[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = min_out complete

    if (!enable) flush = true;

    min_pipeline_stage(segA_in, segB_in, min_out, flush);

    BcsrEle_t cas_vec[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = cas_vec complete

COPY_MIN_TO_CAS:
    for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
    {
#pragma HLS UNROLL
        cas_vec[i] = min_out[i];
    }

    cas_pipeline_stage(cas_vec);

    BcsrEle_t bitmap_vec[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = bitmap_vec complete
    bool fin_flag;

    BitmapStageIO bs_io{.in_vec = cas_vec,
                        .in_valid = true,
                        .opInt = is_intersection,
                        .flush = flush,
                        .out_ready = true,
                        .out_vec = bitmap_vec,
                        .out_fin = &fin_flag,
                        .out_valid = nullptr,
                        .in_ready = nullptr};
    bitmap_pipeline_stage(bs_io);

    compact_pipeline_stage(bitmap_vec, out_compact, out_len, out_popcount);

    out_fin = fin_flag;
}