#include <iostream>
#include <queue>
#include <vector>
#include <limits>

// Define a constant representing a very large value
const int kInf = 1e6;

// Struct representing a connection in the network
struct Connection {
    int destination; // Destination vertex
    int weight;      // Weight (cost) of the connection
};

// Class representing a directed network
class Network {
private:
    int total_nodes_;                      // Total number of nodes
    std::vector<std::vector<Connection>> adj_list_; // Adjacency list for the network

public:
    // Constructor initializes the network with the given number of nodes
    explicit Network(int nodes) : total_nodes_(nodes), adj_list_(nodes + 1) {}

    // Adds a directed connection to the network
    void AddConnection(int start, int end, int weight) {
        adj_list_[start].push_back({end, weight});
    }

    // Returns the adjacency list of the network
    const std::vector<std::vector<Connection>>& GetAdjList() const {
        return adj_list_;
    }

    // Returns the total number of nodes in the network
    int GetTotalNodes() const {
        return total_nodes_;
    }
};

// Function implementing Dijkstra's algorithm with a constraint on the number of hops
int ShortestPathWithHops(int nodes, const Network& network, int max_hops, int source, int target) {
    // Create a 2D vector to track the shortest paths with up to `max_hops` hops
    std::vector<std::vector<int>> min_cost(nodes + 1, std::vector<int>(max_hops + 1, kInf));
    min_cost[source][0] = 0;

    // Priority queue to store {current cost, {current node, hops used}}
    std::priority_queue<std::pair<int, std::pair<int, int>>,
                        std::vector<std::pair<int, std::pair<int, int>>>,
                        std::greater<>> priority_queue;

    // Start from the source node
    priority_queue.push({0, {source, 0}});

    while (!priority_queue.empty()) {
        int current_cost = priority_queue.top().first;      // Current cost
        int current_node = priority_queue.top().second.first; // Current node
        int hops_used = priority_queue.top().second.second; // Hops used so far
        priority_queue.pop();

        // If the target node is reached, return the cost
        if (current_node == target) {
            return current_cost;
        }

        // If more hops can be used
        if (hops_used < max_hops) {
            // Explore all outgoing connections
            for (const Connection& connection : network.GetAdjList()[current_node]) {
                int new_cost = current_cost + connection.weight;
                if (new_cost < min_cost[connection.destination][hops_used + 1]) {
                    min_cost[connection.destination][hops_used + 1] = new_cost;
                    priority_queue.push({new_cost, {connection.destination, hops_used + 1}});
                }
            }
        }
    }

    // If no valid path is found, return -1
    return -1;
}

int main() {
    int nodes;      // Number of nodes
    int connections; // Number of connections
    int max_hops;   // Maximum number of hops
    int source;     // Source node
    int target;     // Target node

    // Input the number of nodes, connections, max hops, source, and target nodes
    std::cin >> nodes >> connections >> max_hops >> source >> target;

    // Initialize the network
    Network network(nodes);

    // Input the connections of the network
    for (int i = 0; i < connections; ++i) {
        int start, end, weight;
        std::cin >> start >> end >> weight;
        network.AddConnection(start, end, weight);
    }

    // Run the shortest path algorithm with hops constraint and print the result
    int result = ShortestPathWithHops(nodes, network, max_hops, source, target);
    std::cout << result;

    return 0;
}
