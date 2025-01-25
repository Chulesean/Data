#include <iostream>
#include <vector>

const int kMaxNodes = 1e5;       // Maximum number of nodes
const int kMaxDepth = 20;        // Maximum depth for binary lifting

class BinaryTree {
private:
    std::vector<std::vector<int>> connections_; // Stores the tree structure
    std::vector<int> time_in_, time_out_;       // Node entry and exit times
    std::vector<std::vector<int>> lift_;        // Ancestor table for binary lifting
    int current_time_;                          // Tracks DFS timing

    // Recursive DFS to populate ancestor table and calculate entry/exit times
    void Traverse(int node, int parent = 0) {
        time_in_[node] = current_time_++;

        lift_[node][0] = parent; // Direct parent
        for (int level = 1; level < kMaxDepth; level++) {
            lift_[node][level] = lift_[lift_[node][level - 1]][level - 1]; // Fill ancestor table
        }

        for (int neighbor : connections_[node]) {
            if (neighbor != parent) {
                Traverse(neighbor, node);
            }
        }

        time_out_[node] = current_time_++;
    }

    // Checks if `ancestor` is an ancestor of `descendant`
    bool AncestorCheck(int ancestor, int descendant) {
        return (time_in_[ancestor] <= time_in_[descendant]) && (time_out_[ancestor] >= time_out_[descendant]);
    }

public:
    explicit BinaryTree(int size) {
        connections_.resize(size);
        time_in_.resize(size);
        time_out_.resize(size);
        lift_.resize(size, std::vector<int>(kMaxDepth));
        current_time_ = 0;
    }

    // Link a child to its parent
    void LinkNodes(int parent, int child) {
        connections_[parent].push_back(child);
    }

    // Prepare the tree for Lowest Common Ancestor (LCA) queries
    void InitializeTree() {
        Traverse(0); // Start DFS from root node (0)
    }

    // Compute the LCA of two nodes
    int ComputeLCA(int first, int second) {
        if (AncestorCheck(first, second)) {
            return first;
        }
        if (AncestorCheck(second, first)) {
            return second;
        }

        for (int level = kMaxDepth - 1; level >= 0; level--) {
            if (!AncestorCheck(lift_[first][level], second)) {
                first = lift_[first][level];
            }
        }
        return lift_[first][0]; // Return the parent of the last valid ancestor
    }
};

int main() {
    int num_nodes, num_queries;
    std::cin >> num_nodes >> num_queries;

    BinaryTree tree(num_nodes);

    // Read the parent-child relationships
    for (int i = 1; i < num_nodes; i++) {
        int parent;
        std::cin >> parent;
        tree.LinkNodes(parent, i);
    }

    tree.InitializeTree(); // Precompute ancestor relationships

    int first_node, second_node, multiplier_x, multiplier_y, increment_z;
    std::cin >> first_node >> second_node >> multiplier_x >> multiplier_y >> increment_z;

    long long total_lca_sum = 0; // Accumulate the LCA results
    int current_lca = 0;         // Holds the last computed LCA

    // Process all queries
    for (int query = 0; query < num_queries; query++) {
        current_lca = tree.ComputeLCA((first_node + current_lca) % num_nodes, second_node); // Calculate LCA
        total_lca_sum += current_lca;

        // Update `first_node` and `second_node` for the next iteration
        first_node = (1LL * multiplier_x * first_node + 1LL * multiplier_y * second_node + increment_z) % num_nodes;
        second_node = (1LL * multiplier_x * second_node + 1LL * multiplier_y * first_node + increment_z) % num_nodes;
    }

    std::cout << total_lca_sum << std::endl; // Output the total LCA sum
    return 0;
}