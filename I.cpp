#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

struct Edge {
  int destination; 
  double probability; 
};

class Graph {
 private:
  std::vector<std::vector<Edge>> adjacency_list_; 
 public:
  explicit Graph(int nodes) : adjacency_list_(nodes + 1) {}

  void AddEdge(int from, int to, double probability) {
    adjacency_list_[from].push_back({to, probability});
    adjacency_list_[to].push_back({from, probability});
  }

  const std::vector<std::vector<Edge>>& GetAdjacencyList() const {
    return adjacency_list_;
  }
};

double Dijkstra(const std::vector<std::vector<Edge>>& graph, int start_node,
                int end_node) { 
  std::vector<double> min_probability(graph.size(), 1); 
  min_probability[start_node] = 0;

  std::priority_queue<std::pair<double, int>,
                      std::vector<std::pair<double, int>>,
                      std::greater<std::pair<double, int>>>
      priority_queue; 
  priority_queue.push({0, start_node});

  while (!priority_queue.empty()) {
    double current_probability = priority_queue.top().first; 
    int current_node = priority_queue.top().second; 
    priority_queue.pop();

    for (const Edge& edge : graph[current_node]) {
      double new_probability = current_probability + edge.probability -
                               current_probability * edge.probability;
      if (new_probability < min_probability[edge.destination]) {
        min_probability[edge.destination] = new_probability;
        priority_queue.push({new_probability, edge.destination});
      }
    }
  }

  return min_probability[end_node];
}

int main() {
  int num_nodes; 
  int num_edges; 
  int start_node; 
  int end_node; 
  std::cin >> num_nodes >> num_edges >> start_node >> end_node;

  Graph graph(num_nodes);

  for (int i = 0; i < num_edges; ++i) {
    int source_node; 
    int target_node; 
    double probability_percentage; 
    std::cin >> source_node >> target_node >> probability_percentage;
    graph.AddEdge(source_node, target_node, probability_percentage / 100);
  }

  const auto& adjacency_list = graph.GetAdjacencyList();
  double result = Dijkstra(adjacency_list, start_node, end_node);

  std::cout << std::setprecision(2) << result << std::endl;

  return 0;
}