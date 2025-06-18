# run.tcl
open_project -reset set_int_kernel_proj
set_top set_int_kernel

# 源文件 + 头文件搜索路径
add_files -cflags "-I./include" {src/kernel.cpp src/bcsr.cpp}

# 若有 testbench
add_files -tb -cflags "-I./include" src/main.cpp

open_solution -reset solution1 -flow_target vitis

# Define technology and clock rate
set_part {xcu55c-fsvh2892-2L-e}
create_clock -period 5 -name default

# Set variable to select which steps to execute
set hls_exec 1

# Run C simulation
csim_design

# Set any optimization directives
# End of directives

if {$hls_exec == 1} {
    # Run Synthesis and Exit
    csynth_design
  
    
} elseif {$hls_exec == 2} {
    # Run Synthesis, RTL Simulation and Exit
    csynth_design
    
    cosim_design
} elseif {$hls_exec == 3} { 
    # Run Synthesis, RTL Simulation, RTL implementation and Exit
    csynth_design
    
    cosim_design
    export_design -format xo -rtl verilog -output set_int_kernel.xo
} else {
    # Default is to exit after setup
    csynth_design
}
config_export -version 2.0.1
#export_design -format ip_catalog -output set_int_kernel_ip 
export_design -format xo -rtl verilog -output set_int_kernel.xo


close_solution
close_project
exit
