| 阶段 | 目标                                 | 关键组件                               |
| -- | ---------------------------------- | ---------------------------------- |
| 1  | 搭建基础的 3CF（3-condition filter）流水线原型 | `main.cpp` + `match_3cf()`         |
| 2  | 实现并测试 Set Intersection Primitive   | `set_intersect()`                  |
| 3  | 设计图数据的输入格式与内存布局                    | 邻接表 `adj[]` 与顶点索引表 `offset[]`      |
| 4  | 集成所有模块，实现端到端的 HLS 模拟运行             | `match_3cf()` 调用 `set_intersect()` |
| 5  | 使用 Vivado HLS 编译并部署至 FPGA 平台       | 完整 `vivado_hls` 工程与综合、实现流程         |
