#ifndef CONFIG_H
#define CONFIG_H

// 硬件相关配置
#define VERTEX_ID_WIDTH 32
#define EDGE_ID_WIDTH 32

// 图大小限制
#define MAX_VERTICES 10000
#define MAX_EDGES 100000
#define MAX_MATCHES 10000

// 并行度配置
#define PARALLEL_UNITS 8

#endif // CONFIG_H
