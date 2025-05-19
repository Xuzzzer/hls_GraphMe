# 设置项目名称和顶层函数
set project_name "GraphHLSMatcher"
set top_function "match_3cf_kernel"

# 设置项目目录
set project_dir "./vivado_hls_project"
open_project -reset $project_dir/$project_name

# 设置时钟周期（单位：ns，对应100MHz）
set clock_period 10

# 创建HLS项目
open_project -reset $project_name

# 添加设计文件
add_files "hls/match_3cf.cpp hls/set_intersect.cpp" -cflags "-std=c++0x -I./include"

# 添加 Xilinx OpenCL 头文件路径
set xilinx_opencl_include "/data-hdd/opt/Xilinx/Vitis_HLS/2023.2/include"

# 添加测试台文件
# add_files -tb "host/main.cpp" -cflags "-std=c++0x -I./include -I$xilinx_opencl_include -DSIMULATION_MODE"
add_files -tb "hls/test_match_3cf.cpp" -cflags "-std=c++0x -I. -I./include"

# 设置顶层函数
set_top $top_function

# 创建解决方案
open_solution -reset "solution1"

# 设置目标设备 (根据您的FPGA型号调整)
set_part {xc7z020clg484-1}

# 设置时钟周期
create_clock -period $clock_period

# 设置指令
config_interface -m_axi_addr64=false
config_rtl -reset_level low

# C仿真
csim_design -clean

# C综合
csynth_design

# RTL协同仿真
cosim_design -trace_level all -tool xsim

# 导出RTL
export_design -format ip_catalog -rtl verilog

# 生成报告
puts "================================================================"
puts "HLS综合报告:"
puts "================================================================"
puts "时钟周期: $clock_period ns"
report_timing_summary
report_utilization

# 关闭项目
close_project

puts "================================================================"
puts "HLS项目 $project_name 已完成所有步骤"
puts "================================================================"