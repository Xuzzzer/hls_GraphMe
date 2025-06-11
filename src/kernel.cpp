#include "bcsr.h"

// Corrected task1_load
void task1_load(BCSR_vec *setA_in, BCSR_vec *setB_in, hls::stream<BCSR_vec_last> &streamA,
                hls::stream<BCSR_vec_last> &streamB, int A_batches, int B_batches)
{
    int max = A_batches > B_batches ? A_batches : B_batches;
LOAD_LOOP:
    for (int i = 0; i < max; ++i)
    {
#pragma HLS PIPELINE II = 1
        BCSR_vec_last vecA, vecB;

        if (i < A_batches)
        {
            vecA.data = setA_in[i];
            vecA.last = (i == A_batches - 1);
            streamA.write(vecA);
        }

        if (i < B_batches)
        {
            vecB.data = setB_in[i];
            vecB.last = (i == B_batches - 1);
            streamB.write(vecB);
        }
    }
}

// Corrected task3_store
void task3_store(hls::stream<BCSR_vec_last> &stream_out, hls::stream<int> &final_popcount,
                 BCSR_vec *o3_stream, int *o3_popcount) // FIX: Correct pointer type
{
#pragma HLS INLINE off
    int i = 0;

STORE_LOOP:
    while (true)
    {
#pragma HLS PIPELINE II = 1
        BCSR_vec_last vec = stream_out.read();
        o3_stream[i++] = vec.data; // FIX: Correct memory write
        if (vec.last)
        {
            *o3_popcount = final_popcount.read();
            break;
        }
    }
}

extern "C"
{
    void set_int_kernel(BCSR_vec *setA_in, BCSR_vec *setB_in, int A_batches, int B_batches,
                        BCSR_vec *o3_stream, int *o3_popcount)
    {
// Interface pragmas are now correct for the pointer types
#pragma HLS INTERFACE m_axi port = setA_in offset = slave bundle = gmemA
#pragma HLS INTERFACE m_axi port = setB_in offset = slave bundle = gmemB
#pragma HLS INTERFACE m_axi port = o3_stream offset = slave bundle = gmemC
#pragma HLS INTERFACE m_axi port = o3_popcount offset = slave bundle = gmemD
#pragma HLS INTERFACE s_axilite port = setA_in bundle = control
#pragma HLS INTERFACE s_axilite port = setB_in bundle = control
#pragma HLS INTERFACE s_axilite port = A_batches bundle = control
#pragma HLS INTERFACE s_axilite port = B_batches bundle = control
#pragma HLS INTERFACE s_axilite port = o3_stream bundle = control
#pragma HLS INTERFACE s_axilite port = o3_popcount bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

#pragma HLS DATAFLOW

        hls::stream<BCSR_vec_last> streamA("streamA");
#pragma HLS STREAM type = fifo variable = streamA

        hls::stream<BCSR_vec_last> streamB("streamB");
#pragma HLS STREAM type = fifo variable = streamB

        hls::stream<BCSR_vec_last> stream_out("stream_out");
#pragma HLS STREAM variable = stream_out

        hls::stream<int> final_popcount("final_popcount");
#pragma HLS STREAM variable = final_popcount

        task1_load(setA_in, setB_in, streamA, streamB, A_batches, B_batches);

        bitonic_stage(streamA, streamB, true, stream_out, final_popcount);

        task3_store(stream_out, final_popcount, o3_stream, o3_popcount);
    }
}