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

static void swap_bcsr_ele(BcsrEle_t &e1, BcsrEle_t &e2)
{
#pragma HLS INLINE
    BcsrEle_t t = e1;
    e1 = e2;
    e2 = t;
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

template <int DEPTH = 2>
static void min_stage_fifo(const BCSR_t new_segA_in[PROCESSING_SEGMENT_SIZE], int lenA,
                           const BCSR_t new_segB_in{PROCESSING_SEGMENT_SIZE}, int lenB,
                           BcsrEle_t min_stage_out[PROCESSING_SEGMENT_SIZE])
{
#pragma HLS INLINE off
#pragma HLS DATAFLOW

    static Bcsr_t fifoA[DEPTH][PROCESSING_SEGMENT_SIZE];
    static Bcsr_t fifoB[DEPTH][PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = fifoA complete dim = 2
#pragma HLS ARRAY_PARTITION variable = fifoB complete dim = 2

    static ap_uint<1> headA = 0, headB = 0;
    static ap_uint<1> tailA = 1, tailB = 1;

    bool consumed_A[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = consumed_A complete dim = 1
    bool consumed_B[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = consumed_B complete dim = 1

    BcsrEle_t core_out[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = core_out complete dim = 1

    min_stage_core(fifoA[headA], fifoB[headB], core_out, consumed_A, consumed_B);

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
}

//// --------------CAS Stage (Single)---------------
static void cas_stage_core(BcsrEle_t vec[PROCESSING_SEGMENT_SIZE], int level)
{
#pragma HLS INLINE
    const int stride = 1 << level;
    const int num_seg = PROCESSING_SEGMENT_SIZE / (2 * stride);

CAS_SEG_LOOP:
    for (int seg = 0; seg < num_seg; ++seg) //seg
    {
#pragma HLS UNROLL
    CAS_STAGE_STRIDE_LOOP:
        for (j = 0; j < stride; ++j)
        {
#pragma HLS UNROLL
            const int lIdx = seg * 2 * stride + stride;
            const int rIdx = lIdx + STRIDE;

            BcsrEle_t l = vec[lIdx];
            BcsrEle_t r = vec[rIdx];

            bool l_is_pad = l.bcsr.isPadding();
            bool r_is_pad = l.bcsr.isPadding();

            bool pick_L = r_is_pad || (!l_is_pad && (l.bcsr.index < r.bcsr.index ||
                                                     (l.bcsr.index == r.bcsr.index && l.fromA)));

            bool swap = !pick_L; // 如果 L 不该在左，就交换
            if (swap)
            {
                vec[lIdx] = r;
                vec[rIdx] = l;
            }
            else
            {
                vec[lIdx] = l;
                vec[rIdx] = r;
            }

            //matched
            bool set_matched_flag = false;
            if (!l_is_pad && !r_is_pad && l.bcsr.index == r.bcsr.index) { set_matched_flag = true; }

            vec[lIdx].matched = set_matched_flag;
            vec[rIdx].matched = set_matched_flag;
        }
    }
}

static void cas_pipeline_stage(BcsrEle_t vec[PROCESSING_SEGMENT_SIZE])
{
#pragma HLS INLINE
CAS_PIPELINE_LEVEL_LOOP:
    for (int level = 0; level < LOG2_PRCESSING_SEGMENT_SIZE; ++level)
    {
#pragma HLS UNROLL
        cas_stage_level(vec, level);
    }
}