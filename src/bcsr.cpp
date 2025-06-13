#include "bcsr.h"

//-----------------------------stage_min---------------------------------------------

void single_cas_stage(const BcsrEle_vec_last &in_buf, BcsrEle_vec_last &out_buf, int level)
{
#pragma HLS INLINE

    const int stride = 1 << level;
    const int num_seg = PROCESSING_SEGMENT_SIZE / (2 * stride);
    out_buf.last = in_buf.last;

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

            BcsrEle_t l = in_buf.data.lane[lIdx];
            BcsrEle_t r = in_buf.data.lane[rIdx];

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

            out_buf.data.lane[lIdx] = l_new;
            out_buf.data.lane[rIdx] = r_new;
        }
    }
}

void bitonic_stage(hls::stream<BCSR_vec_last> &segA_in, hls::stream<BCSR_vec_last> &segB_in,
                   bool op_is_intersection, hls::stream<BCSR_vec_last> &stream_out,
                   hls::stream<int> &total_popcount)
{
#pragma HLS INLINE off
#pragma HLS DATAFLOW

    static BCSR_t buffer_a[DEPTH][PROCESSING_SEGMENT_SIZE];
    static BCSR_t buffer_b[DEPTH][PROCESSING_SEGMENT_SIZE];
    static bool valid_a[DEPTH][PROCESSING_SEGMENT_SIZE];
    static bool valid_b[DEPTH][PROCESSING_SEGMENT_SIZE];

#pragma HLS ARRAY_PARTITION variable = buffer_a complete
#pragma HLS ARRAY_PARTITION variable = buffer_b complete
#pragma HLS ARRAY_PARTITION variable = valid_a complete
#pragma HLS ARRAY_PARTITION variable = valid_b complete

    static bool a_stream_ended = false;
    static bool b_stream_ended = false;
    static bool first_cycle = true;

#ifndef __SYNTHESIS__
    int m = 0;
    int can_num = 0;
#endif

    while (true)
    {
#pragma HLS PIPELINE II = 1

        if (first_cycle)
        {
            for (int d = 0; d < DEPTH; d++)
            {
                for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
                {
#pragma HLS UNROLL
                    valid_a[d][i] = false;
                    valid_b[d][i] = false;
                }
            }
            first_cycle = false;
        }

        bool all_ready_A = true, all_ready_B = true;
        for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
        { //判断此时是否能读新数据
#pragma HLS UNROLL
            all_ready_A &= !valid_a[1][i];
            all_ready_B &= !valid_b[1][i];
        }

#ifndef __SYNTHESIS__
        int readA = 0;
        printf("本次读取数据前是否All_ready_A=%d\n", all_ready_A);
#endif

        if (!segA_in.empty() && all_ready_A)
        //!a_stream_ended && all_ready_A &&
        {
            BCSR_vec_last vecA = segA_in.read();
            readA++;
            a_stream_ended = vecA.last;
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
            {
#pragma HLS UNROLL
                if (!valid_a[0][i])
                {

                    buffer_a[0][i] = vecA.data.lane[i];
                    if (!vecA.data.lane[i].isPadding()) valid_a[0][i] = true;
                    valid_a[1][i] = false;
                }
                else
                {
                    buffer_a[1][i] = vecA.data.lane[i];
                    if (!vecA.data.lane[i].isPadding()) valid_a[1][i] = true;
                }
            }
        }
        else if (all_ready_A) // if (all_ready_A && segA_in.empty())
        {
            //加入Padding
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
            {
#pragma HLS UNROLL
                if (!valid_a[0][i])
                {
                    buffer_a[0][i] = make_padding_bcsr();
                    valid_a[0][i] = false;
                }
                else
                {
                    buffer_a[1][i] = make_padding_bcsr();
                    valid_a[1][i] = false;
                }
            }
        }

        if (all_ready_B && !segB_in.empty())
        {
            BCSR_vec_last vecB = segB_in.read();
            b_stream_ended = vecB.last;
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
            {
#pragma HLS UNROLL
                if (!valid_b[0][i])
                {
                    buffer_b[0][i] = vecB.data.lane[i];
                    if (!vecB.data.lane[i].isPadding()) valid_b[0][i] = true;
                    valid_b[1][i] = false;
                }
                else
                {
                    buffer_b[1][i] = vecB.data.lane[i];
                    if (!vecB.data.lane[i].isPadding()) valid_b[1][i] = true;
                }
            }
        }
        else if (all_ready_B) //if (all_ready_B && segB_in.empty())
        {                     //加入Padding
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
            {
#pragma HLS UNROLL
                if (!valid_b[0][i])
                {
                    buffer_b[0][i] = make_padding_bcsr();
                    valid_b[0][i] = false;
                }
                else
                {
                    buffer_b[1][i] = make_padding_bcsr();
                    valid_b[1][i] = false;
                }
            }
        }

        printf("读取数据后的data情况\n");
        for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
        {
            printf("buffer_a[0][%d]=%u, buffer_b[0][%d]=%u\n", i,
                   (unsigned int)(buffer_a[0][i].index), i, (unsigned int)(buffer_b[0][i].index));
        }
        printf("读取后BUFFER data情况\n");

        for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
        {
            printf("buffer_a[1][%d]=%u, buffer_b[1][%d]=%u\n", i,
                   (unsigned int)(buffer_a[1][i].index), i, (unsigned int)(buffer_b[1][i].index));
        }

        printf("读取数据后的valid情况\n");
        for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
        {
            printf("valid_a[0][%d]=%d, valid_b[0][%d]=%d\n", i, valid_a[0][i], i, valid_b[0][i]);
        }
        printf("读取后BUFFER valid情况\n");
        for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
        {
            printf("valid_a[1][%d]=%d, valid_b[1][%d]=%d\n", i, valid_a[1][i], i, valid_b[1][i]);
        }

        bool can_finish = is_all_padding(buffer_a[0]) && is_all_padding(buffer_b[0]);

        can_num++;
        if (!can_finish) { printf("这是第%d次可以处理\n", can_num); }
        else
        {
            printf("第%d次读取数据更新后不再处理\n", can_num);
            break;
        }

        if (!can_finish)
        {
#pragma HLS DATAFLOW
            printf("can_proces=true 本次开始处理了-------------\n");
            //min_core
            bool chooseA[PROCESSING_SEGMENT_SIZE];
#pragma HLS ARRAY_PARTITION variable = chooseA complete
            BcsrEle_vec_last out1_vec_last;

        compare_and_output_loop:
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
#pragma HLS UNROLL
                int j = PROCESSING_SEGMENT_SIZE - 1 - i;

                const BCSR_t &ele_a = buffer_a[0][i];
                const BCSR_t &ele_b = buffer_b[0][j];
                bool minA = ele_b.isPadding() || (!ele_a.isPadding() && ele_a.index <= ele_b.index);

                out1_vec_last.data.lane[i].bcsr = minA ? ele_a : ele_b;
                out1_vec_last.data.lane[i].fromA = minA;
                out1_vec_last.data.lane[i].matched = false;
                chooseA[i] = minA;
            }

            printf("本次处理后chooseA的情况\n");
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
                printf("chooseA[%d]=%d\n", i, chooseA[i]);
            }

            out1_vec_last.last = a_stream_ended && b_stream_ended;

        buffer_update_loop:
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
#pragma HLS UNROLL
                int j = PROCESSING_SEGMENT_SIZE - 1 - i;
                if (chooseA[i])
                {
                    if (valid_a[1][i])
                    {
                        buffer_a[0][i] = buffer_a[1][i];
                        buffer_a[1][i] = make_padding_bcsr();
                        valid_a[1][i] = false;
                        valid_a[0][i] = true;
                    }
                    else
                    {
                        valid_a[0][i] = false;
                        buffer_a[0][i] = make_padding_bcsr();
                    }
                }
                else
                {
                    if (valid_b[1][j])
                    {
                        buffer_b[0][j] = buffer_b[1][j];
                        buffer_b[1][j] = make_padding_bcsr();
                        valid_b[1][j] = false;
                        valid_b[0][j] = true;
                    }
                    else
                    {
                        valid_b[0][j] = false;
                        buffer_b[0][j] = make_padding_bcsr();
                    }
                }
            }

            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
                printf("valid_a[0][%d] = %d, valid_b[0][%d] = %d\n", i, valid_a[0][i], i,
                       valid_b[0][i]);
            }
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
                printf("valid_a[1][%d] = %d, valid_b[1][%d] = %d\n", i, valid_a[1][i], i,
                       valid_b[1][i]);
            }

            //stage2
            BcsrEle_vec_last temp_vec[LOG2_PRCESSING_SEGMENT_SIZE + 1];

            temp_vec[0] = out1_vec_last;
            for (int i = 0; i < LOG2_PRCESSING_SEGMENT_SIZE; ++i)
            {
                single_cas_stage(temp_vec[i], temp_vec[i + 1], LOG2_PRCESSING_SEGMENT_SIZE - i - 1);
            }

            //stage3
            BcsrEle_vec_last &out2_vec_last = temp_vec[LOG2_PRCESSING_SEGMENT_SIZE];

            static BcsrEle_t last_reg;
#pragma HLS RESET variable = last_reg
            BcsrEle_vec_last out3_vec_last;
            out3_vec_last.last = out2_vec_last.last;

        BITMAP_PROCESS_LOOP:
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
#pragma HLS UNROLL
                const BcsrEle_t &l = (i == 0) ? last_reg : out2_vec_last.data.lane[i - 1];
                const BcsrEle_t &r = out2_vec_last.data.lane[i];
                BcsrEle_t &out_elem = out3_vec_last.data.lane[i];
                out_elem = l;

                bool matched =
                    !l.bcsr.isPadding() && !r.bcsr.isPadding() && (l.bcsr.index == r.bcsr.index);

                if (op_is_intersection)
                {
                    out_elem.bcsr.bitmap =
                        matched ? (l.bcsr.bitmap & r.bcsr.bitmap) : bcsr_bitmap_t(0);
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
            }

            last_reg = out2_vec_last.data.lane[PROCESSING_SEGMENT_SIZE - 1];

            //stage4
            BcsrEle_vec_last out4_vec_last;
            out4_vec_last.last = out3_vec_last.last;
            BCSR_vec_last out4_pack;

            int wr = 0;
            int pc_sum = 0;
            static int totol_pop = 0;

        COMPACT_SCAN_LOOP:
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; i++)
            {
                const BcsrEle_t &e = out3_vec_last.data.lane[i];
                bool keep = (e.bcsr.bitmap != 0);
                if (keep)
                {
                    out4_vec_last.data.lane[wr++] = e;
                    pc_sum += e.bcsr.popcount();
                }
            }

        PADDING_FILL_LOOP:
            for (int k = wr; k < PROCESSING_SEGMENT_SIZE; ++k)
            {
#pragma HLS UNROLL
                out4_vec_last.data.lane[k].bcsr = make_padding_bcsr(); // bitmap=0 Padding->width
                out4_vec_last.data.lane[k].fromA = false;
                out4_vec_last.data.lane[k].matched = false;
            }

            totol_pop += pc_sum;

            out4_pack.last = out4_vec_last.last;
        PACK_LOOP:
            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
#pragma HLS UNROLL
                out4_pack.data.lane[i] = out4_vec_last.data.lane[i].bcsr;
            }
            printf("last4 = %d\n", (int)(out4_vec_last.last));

            stream_out.write(out4_pack);
            total_popcount.write(totol_pop);
            int p = 0;
            p++;

            if (out4_vec_last.last)
            {
                //done = true;
                printf("p=%d\n", p);
                printf("out4_last = %d\n", (int)(out4_vec_last.last));
            }
        }

        bool buffers_empty = true; // 假设本轮缓冲区已空
        for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
        {
#pragma HLS UNROLL
            if (valid_a[0][i] || valid_b[0][i])
            {
                printf("hai you shu ju \n");
                buffers_empty = false; // 发现还有数据
                break;                 // 提前结束扫描即可
            }
        }
    }
}
/*
void bitonic_stage(hls::stream<BCSR_vec_last> &segA_in, hls::stream<BCSR_vec_last> &segB_in,
                   bool op_is_intersection, hls::stream<BCSR_vec_last> &stream_out,
                   hls::stream<int> &total_popcount)
{

    // 存储所有A和B的元素
    static BCSR_t elements_A[64]; // 假设最大64个元素
    static BCSR_t elements_B[64];

    int count_A = 0, count_B = 0;
    int total_pop = 0;

    // 读取所有A的元素
    bool a_done = false;
    while (!a_done)
    {
        if (!segA_in.empty())
        {
            BCSR_vec_last vecA = segA_in.read();

            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
                if (!vecA.data.lane[i].isPadding()) { elements_A[count_A++] = vecA.data.lane[i]; }
            }

            if (vecA.last) a_done = true;
        }
    }

    // 读取所有B的元素
    bool b_done = false;
    while (!b_done)
    {
        if (!segB_in.empty())
        {
            BCSR_vec_last vecB = segB_in.read();

            for (int i = 0; i < PROCESSING_SEGMENT_SIZE; ++i)
            {
                if (!vecB.data.lane[i].isPadding()) { elements_B[count_B++] = vecB.data.lane[i]; }
            }

            if (vecB.last) b_done = true;
        }
    }

    printf("Read %d elements from A, %d elements from B\n", count_A, count_B);

    // 排序合并算法进行交集计算
    int i = 0, j = 0;
    BCSR_vec_last output_vec;
    int output_count = 0;

    // 初始化输出向量
    for (int k = 0; k < PROCESSING_SEGMENT_SIZE; k++)
    {
        output_vec.data.lane[k] = make_padding_bcsr();
    }

    while (i < count_A && j < count_B)
    {
        if (elements_A[i].index == elements_B[j].index)
        {
            // 找到匹配的索引，计算交集
            BCSR_t intersection;
            intersection.index = elements_A[i].index;
            intersection.bitmap = elements_A[i].bitmap & elements_B[j].bitmap;

            int popcount = __builtin_popcount(intersection.bitmap);
            total_pop += popcount;

            printf("Intersection found: idx=%d, bitmap_A=%02x, bitmap_B=%02x, result=%02x, "
                   "popcount=%d\n",
                   intersection.index, elements_A[i].bitmap, elements_B[j].bitmap,
                   intersection.bitmap, popcount);

            // 将结果放入输出向量
            output_vec.data.lane[output_count % PROCESSING_SEGMENT_SIZE] = intersection;
            output_count++;

            // 如果当前向量填满了，输出
            if (output_count % PROCESSING_SEGMENT_SIZE == 0)
            {
                output_vec.last = false;
                stream_out.write(output_vec);

                // 重置输出向量
                for (int k = 0; k < PROCESSING_SEGMENT_SIZE; k++)
                {
                    output_vec.data.lane[k] = make_padding_bcsr();
                }
            }

            i++;
            j++;
        }
        else if (elements_A[i].index < elements_B[j].index) { i++; }
        else { j++; }
    }

    // 输出最后的向量（如果有剩余元素）
    if (output_count % PROCESSING_SEGMENT_SIZE != 0 || output_count == 0)
    {
        output_vec.last = true;
        stream_out.write(output_vec);
    }
    else
    {
        // 如果刚好填满，需要发送一个空的last向量
        for (int k = 0; k < PROCESSING_SEGMENT_SIZE; k++)
        {
            output_vec.data.lane[k] = make_padding_bcsr();
        }
        output_vec.last = true;
        stream_out.write(output_vec);
    }

    // 输出总的popcount
    total_popcount.write(total_pop);
    printf("Total intersection popcount: %d\n", total_pop);
}
    */