# Graph Processing on FPGA  

> ⚙️ 基于 **Xilinx Vitis HLS** 的GPM
> 当前目标：在 FPGA 上加速 **`set_intersect`**，主要是在用双调排序来对bcsr格式的数据做集合交集.

---

## 目录结构

| 目录/文件      | 说明                                          |
| ---------- | ------------------------------------------- |
| `src/`     | **HLS** 内核源代码（`gpm_kernel.cpp` 等）           |
| `include/` | 公共头文件，数据结构与内核接口定义                           |
| `dataset/` | 测试数据集（矩阵 / 图）                               |
| `host.cpp` | Host 端应用，生成exe作为主机端应用            |
| `run.tcl`  | 一键脚本：启动 **Vitis HLS** 仿真→综合→导出              |
| `build/`   | **out‑of‑source** 构建输出（已在 `.gitignore` 中排除） |
| `hls/`     | Vitis HLS 生成的报告 / xclbin / xo 产物（已忽略）       |
| `Makefile` | 也可以用来执行仿真 ｜
|`host_file` |是主机端来预处理文件｜

---

## 里程碑

| 日期             | 进展                                                               |
| -------------- | ---------------------------------------------------------------- |
| **2025‑05‑20** | 修复矩阵遍历计数逻辑。顺时针 1‑2‑3‑4 编号时，忽略对角线 (1‑3, 2‑4) 的连通性判断，确保识别矩阵本身 ✔︎   |
| **2025‑05‑27** | 启动 **BCSR** 改造与 **Bitonic Merge** 接口整理，目标：提高 `set_intersect` 吞吐率 |
| **2025‑06‑18** | 上板实现bcsr流水线对集合交集的处理 |

---

## 快速上手

### 1 · C/RTL 功能仿真（CMake）

```bash

mkdir -p build && cd build
cmake ..
make
./graph_matching

```

### 2 · Vitis HLS 综合与报告

```bash
# 在工程根目录执行
vitis_hls -f run.tcl
```

`run.tcl` 将自动：

1. 创建项目并导入 `src/`、`include/` 文件；
2. 运行 C/RTL 仿真；
3. 综合并生成 `gpm_kernel.xo`、`gpm_kernel.xclbin`；
4. 保存 HTML / XML 报告到 `hls/` 目录。




