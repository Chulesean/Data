#include <iostream>
#include <vector>
#include <string>

const int BITS_PER_INT = 32;

class CustomBitset {
 public:
  explicit CustomBitset(int size)
      : size_(size), bits_((size + BITS_PER_INT - 1) / BITS_PER_INT, 0) {}

  void set(int position) {
    bits_[position / BITS_PER_INT] |= (1u << (position % BITS_PER_INT));
  }

  bool test(int position) const {
    return (bits_[position / BITS_PER_INT] & (1u << (position % BITS_PER_INT))) != 0;
  }

  void operator|=(const CustomBitset& other) {
    for (int i = 0; i < bits_.size(); ++i) {
      bits_[i] |= other.bits_[i];
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const CustomBitset& bitset) {
    for (int i = 0; i < bitset.size_; ++i) {
      os << bitset.test(i);
    }
    return os;
  }

 private:
  int size_;
  std::vector<unsigned int> bits_;
};

class TransitiveClosure {
 public:
  explicit TransitiveClosure(int num_vertices)
      : num_vertices_(num_vertices),
        adjacency_paths_(num_vertices, CustomBitset(num_vertices)) {}

  void AddEdge(int from, int to) {
    adjacency_paths_[from].set(to);
  }

  void ComputeClosure() {
    for (int intermediate = 0; intermediate < num_vertices_; ++intermediate) {
      for (int start = 0; start < num_vertices_; ++start) {
        if (adjacency_paths_[start].test(intermediate)) {
          adjacency_paths_[start] |= adjacency_paths_[intermediate];
        }
      }
    }
  }

  void DisplayPaths() const {
    for (int vertex = 0; vertex < num_vertices_; ++vertex) {
      std::cout << adjacency_paths_[vertex] << std::endl;
    }
  }

 private:
  int num_vertices_;
  std::vector<CustomBitset> adjacency_paths_;
};

int main() {
  int num_vertices;
  std::cin >> num_vertices;

  TransitiveClosure closure(num_vertices);

  for (int row = 0; row < num_vertices; row++) {
    std::string input_row;
    std::cin >> input_row;
    for (int column = 0; column < num_vertices; column++) {
      if (input_row[column] == '1') {
        closure.AddEdge(row, column);
      }
    }
  }

  closure.ComputeClosure();
  closure.DisplayPaths();

  return 0;
}