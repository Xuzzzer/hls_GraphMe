
├── CMakeLists.txt//主要负责c编译，验证功能
├── dataset
│   ├── rectangle.txt
│   ├── triangle.txt
│   └── wiki-vote_input.txt
├── host//读入边列表，去重等一系列操作之后，把CSR给FPGA
│   ├── graph_loader.cpp
│   ├── graph_loader.h
│   └── testbench.cpp
├── include
│   ├── graph.h
│   ├── set_intersect.h
│   └── types.h
├── run.tcl//简单跑的脚本
├── src//hls实现的部分
│   ├── gpm_kernel.cpp
│   ├── graph.cpp
│   ├── schedule.cpp
│   └── set_intersect.cpp
├── todo.md
├── vitis_hls.log

5.20进度：矩阵的数数的逻辑是存在问题的，但是先尝试跑了后续的流程，目前过了前两个仿真，协同仿真还需要通过最终的depth端口的定义，待改善
