cmake_minimum_required(VERSION 3.10)
project(graphme)

set(CMAKE_CXX_STANDARD 17)

# 包含头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# 添加可执行文件：用于 CPU 验证
add_executable(cpu_verify host/cpu_main.cpp src/graph_loader.cpp src/graph.cpp src/common.cpp)