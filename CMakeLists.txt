cmake_minimum_required(VERSION 3.10)
project(GraphMe)

# Set Xilinx HLS include path
set(XILINX_HLS_PATH "/data-hdd/opt/Xilinx/Vitis_HLS/2023.2")

# Set include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/host
    ${XILINX_HLS_PATH}/include
)

# 设置源文件
set(SOURCES
    host/testbench.cpp
    host/graph_loader.cpp
    src/gpm_kernel.cpp
    src/graph.cpp
    src/set_intersect.cpp
)

# 生成可执行文件
add_executable(graph_matching ${SOURCES})

# 编译选项
target_compile_features(graph_matching PRIVATE cxx_std_11)

# Add Xilinx specific definitions 
target_compile_definitions(graph_matching PRIVATE
    SIMULATION_MODE
)