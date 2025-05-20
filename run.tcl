# 设置项目名称和顶层函数
set project_name "GraphMe"
set top_function "gpm_kernel"

# 设置项目目录
set project_dir "./vivado_hls_project"
open_project -reset $project_dir/$project_name

# 设置时钟周期（单位：ns，对应100MHz）
set clock_period 10

# 添加设计文件
add_files "src/gpm_kernel.cpp src/graph.cpp src/set_intersect.cpp" -cflags "-std=c++14 -I./include"

# 添加 Xilinx OpenCL 头文件路径
set xilinx_opencl_include "/data-hdd/opt/Xilinx/Vitis_HLS/2023.2/include"

# 添加测试台文件
add_files -tb "host/testbench.cpp host/graph_loader.cpp" -cflags "-std=c++14 -I./include -I$xilinx_opencl_include"



# 设置顶层函数
set_top $top_function

# 创建解决方案
open_solution -reset "solution1"

# 设置目标设备
set_part {xc7z020clg484-1}

# 设置时钟周期
create_clock -period $clock_period

# 设置接口配置
config_interface -m_axi_addr64=false
config_rtl -reset_level low

# 配置C仿真选项
config_compile -pipeline_loops 0


# C仿真
csim_design  -clean  

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