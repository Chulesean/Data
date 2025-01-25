#include <iostream>
#include <vector>

template <typename T>
class DAGSorter {
 public:
  explicit DAGSorter(int nodes)
      : total_nodes(nodes), adjacency_list(nodes), incoming_edges(nodes, 0) {}

  void AddDependency(T start, T end) {
    adjacency_list[start].push_back(end);
    ++incoming_edges[end];
  }

  bool ExecuteSort(std::vector<int>& order) {
    std::vector<T> zero_incoming_nodes;

    // Collect nodes with zero incoming edges
    for (T i = 0; i < total_nodes; ++i) {
      if (incoming_edges[i] == 0) {
        zero_incoming_nodes.push_back(i);
      }
    }

    int processed_nodes = 0;

    // Process nodes with zero incoming edges
    while (!zero_incoming_nodes.empty()) {
      T current = zero_incoming_nodes.back();
      zero_incoming_nodes.pop_back();
      order.push_back(current);

      for (T adjacent : adjacency_list[current]) {
        if (--incoming_edges[adjacent] == 0) {
          zero_incoming_nodes.push_back(adjacent);
        }
      }

      ++processed_nodes;
    }

    // Return true if all nodes are processed, indicating a valid topological order
    return processed_nodes == total_nodes;
  }

 private:
  T total_nodes;
  std::vector<std::vector<T>> adjacency_list;
  std::vector<int> incoming_edges;
};

int main() {
  int nodes, dependencies;
  std::cin >> nodes >> dependencies;

  DAGSorter<int> graph_sorter(nodes);
  std::vector<int> sorted_nodes;

  // Input dependencies
  for (int i = 0; i < dependencies; ++i) {
    int start, end;
    std::cin >> start >> end;
    graph_sorter.AddDependency(start, end);
  }

  // Perform topological sort
  if (graph_sorter.ExecuteSort(sorted_nodes)) {
    std::cout << "YES" << std::endl;
    for (int node : sorted_nodes) {
      std::cout << node << " ";
    }
  } else {
    std::cout << "NO";
  }

  return 0;
}
