#include "graph_loader.h"

#include <fstream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <utility>

std::unique_ptr<Graph> GraphLoader::load_from_file(const std::string& filename,
                                                   FileFormat format) {
  switch (format) {
    case FileFormat::EDGE_LIST:
      return load_from_edge_list(filename);
    case FileFormat::ADJACENCY_LIST:
      return load_from_adj_list(filename);
    case FileFormat::MATRIX:
      return load_from_matrix(filename);
    default:
      throw std::runtime_error("Unsupported file format");
  }
}

std::unique_ptr<Graph> GraphLoader::load_from_edge_list(
    const std::string& filename) {
  FILE* fp = fopen(filename.c_str(), "r");
  if (!fp) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  int v_cnt;
  unsigned int e_cnt;
  if (fscanf(fp, "%d%u", &v_cnt, &e_cnt) != 2) {
    fclose(fp);
    throw std::runtime_error("Failed to read vertex and edge count");
  }

  // 分配边空间（双向边 * 2），其中next_id代表实际的最终的顶点个数
  std::pair<int, int>* edges = new std::pair<int, int>[e_cnt * 2];
  unsigned int tmp_e = 0;
  int next_id = 0;
  std::unordered_map<int, int> id_map;

  // 分配degree数组
  int* degree = new int[v_cnt];
  memset(degree, 0, sizeof(int) * v_cnt);

  int x, y;
  while (fscanf(fp, "%d%d", &x, &y) == 2) {
    if (x == y) {
      printf("find self circle\n");
      continue;
    }
    if (!id_map.count(x)) id_map[x] = next_id++;
    if (!id_map.count(y)) id_map[y] = next_id++;
    x = id_map[x];
    y = id_map[y];
    edges[tmp_e++] = std::make_pair(x, y);
    edges[tmp_e++] = std::make_pair(y, x);
    ++degree[x];
    ++degree[y];
  }
  fclose(fp);

  // 排序 去重
  std::sort(edges, edges + tmp_e);
  auto unique_end = std::unique(edges, edges + tmp_e);
  unsigned int unique_edges = unique_end - edges;

  // 构建 offset
  unsigned int* vertex_ = new unsigned int[next_id + 1];
  unsigned int idx = 0;
  for (int i = 0; i < next_id; ++i) {
    vertex_[i] = idx;
    idx += degree[i];
  }
  vertex_[next_id] = idx;

  // 创建图
  auto graph = std::make_unique<Graph>(next_id);
  graph->v_cnt = next_id;
  graph->e_cnt = unique_edges;

  // 分配并设置顶点和边数组
  graph->vertex_ = vertex_;
  graph->edge_ = new int[unique_edges];
  for (int i = 0; i < unique_edges; ++i) {
    graph->edge_[i] = edges[i].second;
  }

  // 2或者是指针拷贝，memcpy(graph->edge_, &(edges[0].second), unique_edges *
  // sizeof(int));
  /*3
  int* edge_ = new int[unique_edges];
  std::vector<int> pos(next_id);
  //拷贝vertex_ to pos
  for (int i = 0; i < next_id; ++i) {
  pos[i] = vertex_[i];
  }
  for (i = 0; i < unique_edges; ++i) {
      int u = edges[i].first;
      int v = edges[i].second;
      edge_[pos[u]++] = v;
  }
  */

  // 清理
  delete[] edges;
  delete[] degree;

  return graph;
}

std::unique_ptr<Graph> GraphLoader::load_from_adj_list(
    const std::string& filename) {
  throw std::runtime_error("Not implemented");
}

std::unique_ptr<Graph> GraphLoader::load_from_matrix(
    const std::string& filename) {
  throw std::runtime_error("Not implemented");
}
