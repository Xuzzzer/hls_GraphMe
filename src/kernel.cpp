#include "bcsr.h"

typedef ap_uint<1> flag;

void task1_load(BCSR_t *setA_in, BCSR_t *setB_in, hls::stream<BCSR_t> &streamA,
                hls::stream<BCSR_t> &streamB, int A_elements, int B_elements)
{
LOAD_LOOP:
    for (int i = 0; i < A_elements; ++i)
    {
#pragma HLS PIPELINE II = 1
        if (i < A_elements) streamA.write(setA_in[i]);
        if (i < B_elements) streamB.write(setB_in[i]);
    }
}

void task3_store(hls::stream<BCSR_t> &stream_out, BCSR_t *set_result_out, int A_elements,
                 int B_elements)
{
STORE_LOOP:
    for (int i = 0; i < (A_elements > B_elements ? A_elements : B_elements); ++i)
    {
#pragma HLS PIPELINE II = 1
        set_result_out[i] = stream_out.read();
    }
}

extern "C"
{
    void set_int_kernel(BCSR_t *setA_in, BCSR_t *setB_in, BCSR_t *set_result_out, int A_elements,
                        int B_elements, int *final_popcount)
    {

#pragma HLS INTERFACE m_axi port = setA_in offset = slave bundle = gmemA
#pragma HLS INTERFACE m_axi port = setB_in offset = slave bundle = gmemB
#pragma HLS INTERFACE m_axi port = set_result_out offset = slave bundle = gmemC
#pragma HLS INTERFACE m_axi port = final_popcount offset = slave bundle = gmemD
#pragma HLS INTERFACE s_axilite port = setA_in bundle = control
#pragma HLS INTERFACE s_axilite port = setB_in bundle = control
#pragma HLS INTERFACE s_axilite port = A_elements bundle = control
#pragma HLS INTERFACE s_axilite port = B_elements bundle = control
#pragma HLS INTERFACE s_axilite port = set_result_out bundle = control
#pragma HLS INTERFACE s_axilite port = final_popcount bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

#pragma HLS DATAFLOW

        hls::stream<BCSR_vec> streamA("streamA");
#pragma HLS STREAM type = fifo variable = streamA depth = PROCESSING_SEGMENT_SIZE

        hls::stream<BCSR_vec> streamB("streamB");
#pragma HLS STREAM type = fifo variable = streamB depth = PROCESSING_SEGMENT_SIZE

        hls::stream<BCSR_t> stream_out("stream_out");
#pragma HLS STREAM variable = stream_out depth = PROCESSING_SEGMENT_SIZE

        const bool perform_intersection = true;

        task1_load(setA_in, setB_in, streamA, streamB, A_elements, B_elements);

        set_intersection_pipeline_top(streamA, streamB, stream_out, *final_popcount);

        task3_store(stream_out, set_result_out, A_elements, B_elements);
    }
}