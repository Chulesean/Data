#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

template <typename T>
class DirectedGraph {
 public:
  explicit DirectedGraph(int nodes)
      : num_nodes(nodes),
        forward_adj_list(nodes),
        reverse_adj_list(nodes) {}

  void AddConnection(T start, T end) {
    forward_adj_list[start].push_back(end);
    reverse_adj_list[end].push_back(start);
  }

  std::vector<std::vector<T>> IdentifyStronglyConnectedComponents() {
    std::stack<T> finish_sequence;
    std::vector<bool> is_visited(num_nodes, false);

    // Perform a DFS to record the finish sequence of nodes
    for (T i = 0; i < num_nodes; ++i) {
      if (!is_visited[i]) {
        PerformDepthFirstSearch(i, is_visited, finish_sequence);
      }
    }

    // Reset visited status for the second pass
    is_visited.assign(num_nodes, false);

    std::vector<std::vector<T>> scc_collection;

    // Process nodes in reverse finish sequence to extract SCCs
    while (!finish_sequence.empty()) {
      T current = finish_sequence.top();
      finish_sequence.pop();

      if (!is_visited[current]) {
        std::vector<T> scc;
        ExtractComponent(current, is_visited, scc);
        scc_collection.push_back(scc);
      }
    }

    return scc_collection;
  }

  DirectedGraph<T> CreateCondensedGraph(const std::vector<std::vector<T>>& scc_collection) {
    int compressed_nodes = scc_collection.size();
    DirectedGraph<T> condensed_graph(compressed_nodes);

    for (size_t i = 0; i < scc_collection.size(); ++i) {
      for (T node : scc_collection[i]) {
        for (T neighbor : forward_adj_list[node]) {
          int src_index = GetComponentIndex(scc_collection, node);
          int dest_index = GetComponentIndex(scc_collection, neighbor);
          if (src_index != dest_index) {
            condensed_graph.AddConnection(src_index, dest_index);
          }
        }
      }
    }

    return condensed_graph;
  }

  std::pair<int, int> ComputeZeroDegrees() {
    std::vector<int> incoming_degree(num_nodes, 0);
    std::vector<int> outgoing_degree(num_nodes, 0);

    for (T node = 0; node < num_nodes; ++node) {
      for (T neighbor : forward_adj_list[node]) {
        outgoing_degree[node]++;
        incoming_degree[neighbor]++;
      }
    }

    int zero_incoming = std::count(incoming_degree.begin(), incoming_degree.end(), 0);
    int zero_outgoing = std::count(outgoing_degree.begin(), outgoing_degree.end(), 0);

    return {zero_incoming, zero_outgoing};
  }

 private:
  int num_nodes;
  std::vector<std::vector<T>> forward_adj_list;
  std::vector<std::vector<T>> reverse_adj_list;

  void PerformDepthFirstSearch(T node, std::vector<bool>& is_visited, std::stack<T>& finish_sequence) {
    is_visited[node] = true;

    for (T neighbor : forward_adj_list[node]) {
      if (!is_visited[neighbor]) {
        PerformDepthFirstSearch(neighbor, is_visited, finish_sequence);
      }
    }

    finish_sequence.push(node);
  }

  void ExtractComponent(T node, std::vector<bool>& is_visited, std::vector<T>& scc) {
    is_visited[node] = true;
    scc.push_back(node);

    for (T neighbor : reverse_adj_list[node]) {
      if (!is_visited[neighbor]) {
        ExtractComponent(neighbor, is_visited, scc);
      }
    }
  }

  int GetComponentIndex(const std::vector<std::vector<T>>& scc_collection, T node) const {
    for (size_t i = 0; i < scc_collection.size(); ++i) {
      if (std::find(scc_collection[i].begin(), scc_collection[i].end(), node) != scc_collection[i].end()) {
        return static_cast<int>(i);
      }
    }
    return -1;
  }
};

int main() {
  int total_nodes, total_edges;
  std::cin >> total_nodes >> total_edges;

  DirectedGraph<int> network(total_nodes);

  for (int i = 0; i < total_edges; ++i) {
    int start, end;
    std::cin >> start >> end;
    network.AddConnection(start - 1, end - 1);  // Adjust for 0-based indexing
  }

  // Identify strongly connected components (SCCs)
  auto components = network.IdentifyStronglyConnectedComponents();

  // If there's only one SCC or no edges in the graph
  if (components.size() <= 1) {
    std::cout << 0 << std::endl;
    return 0;
  }

  // Create the condensed graph from SCCs
  DirectedGraph<int> condensed_network = network.CreateCondensedGraph(components);

  // Compute the in-degree and out-degree properties for the condensed graph
  auto zero_degrees = condensed_network.ComputeZeroDegrees();

  // Output the maximum of zero in-degrees or zero out-degrees
  std::cout << std::max(zero_degrees.first, zero_degrees.second) << std::endl;

  return 0;
}
