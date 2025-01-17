#include <climits>
#include <iostream>
#include <queue>
#include <vector>

template <typename T>
class Network {
 public:
  explicit Network(int vertices) { adjacency_list.resize(vertices); }

  void AddConnection(T start, T end) {
    adjacency_list[start].push_back(end);
    adjacency_list[end].push_back(start);
  }

  T ComputeMinimalPath(T first_point, T second_point, T goal, int total_vertices) {
    std::vector<T> distance_from_first(total_vertices, INT_MAX);
    std::vector<T> distance_from_second(total_vertices, INT_MAX);
    std::vector<T> distance_from_goal(total_vertices, INT_MAX);

    PerformBFS(first_point, total_vertices, distance_from_first);
    PerformBFS(second_point, total_vertices, distance_from_second);
    PerformBFS(goal, total_vertices, distance_from_goal);

    return EvaluateMinimalDistance(total_vertices, distance_from_first, distance_from_second, distance_from_goal);
  }

 private:
  std::vector<std::vector<T>> adjacency_list;

  void PerformBFS(T source, int total_vertices, std::vector<T>& distances) {
    std::priority_queue<std::pair<T, T>, std::vector<std::pair<T, T>>, std::greater<>> priority_queue;
    std::vector<bool> processed(total_vertices, false);

    distances[source] = 0;
    priority_queue.emplace(0, source);

    while (!priority_queue.empty()) {
      T current_node = priority_queue.top().second;
      priority_queue.pop();

      if (processed[current_node]) continue;
      processed[current_node] = true;

      for (const T& adjacent : adjacency_list[current_node]) {
        if (distances[current_node] + 1 < distances[adjacent]) {
          distances[adjacent] = distances[current_node] + 1;
          priority_queue.emplace(distances[adjacent], adjacent);
        }
      }
    }
  }

  static T EvaluateMinimalDistance(int total_vertices, const std::vector<T>& dist1,
                                    const std::vector<T>& dist2, const std::vector<T>& dist3) {
    T minimal_distance = INT_MAX;
    for (int i = 0; i < total_vertices; ++i) {
      minimal_distance = std::min(minimal_distance, dist1[i] + dist2[i] + dist3[i]);
    }
    return minimal_distance;
  }
};

int main() {
  int vertices, connections, person_a, person_b, store;
  std::cin >> vertices >> connections >> person_a >> person_b >> store;

  Network<int> road_network(vertices + 1);

  for (int i = 0; i < connections; ++i) {
    int point_x, point_y;
    std::cin >> point_x >> point_y;
    road_network.AddConnection(point_x, point_y);
  }

  std::cout << road_network.ComputeMinimalPath(person_a, person_b, store, vertices + 1);
  return 0;
}