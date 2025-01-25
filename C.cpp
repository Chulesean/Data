#include <algorithm>
#include <iostream>
#include <vector>

const int MAX_NODES = 53;
int graph[MAX_NODES][MAX_NODES];

void performDFS(int node, std::vector<bool>& visited, std::vector<int>& currentComponent) {
    visited[node] = true;
    currentComponent.push_back(node);

    for (int neighbor = 0; neighbor < MAX_NODES; ++neighbor) {
        if (graph[node][neighbor] == 1 && !visited[neighbor]) {
            performDFS(neighbor, visited, currentComponent);
        }
    }
}

std::vector<int> getLargestFullyConnectedGroup(int nodeCount) {
    std::vector<bool> isSubsetValid(1 << nodeCount, false);
    std::vector<int> adjacencyMask(nodeCount, 0);
    std::vector<int> largestGroup;      

    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            if (graph[i][j] == 1) {
                adjacencyMask[i] |= (1 << j);
            }
        }
    }

    for (int subset = 1; subset < (1 << nodeCount); ++subset) {
        if ((subset & (subset - 1)) == 0) { 
            isSubsetValid[subset] = true;
        } else {
            int lastNode = __builtin_ctz(subset); 
            int subsetExcludingLast = subset ^ (1 << lastNode);

            if (isSubsetValid[subsetExcludingLast] &&
                (subsetExcludingLast & adjacencyMask[lastNode]) == subsetExcludingLast) {
                isSubsetValid[subset] = true;
            }
        }
    }

    for (int subset = 1; subset < (1 << nodeCount); ++subset) {
        if (isSubsetValid[subset] && __builtin_popcount(subset) > largestGroup.size()) {
            largestGroup.clear();
            for (int i = 0; i < nodeCount; ++i) {
                if (subset & (1 << i)) {
                    largestGroup.push_back(i);
                }
            }
        }
    }

    return largestGroup;
}

std::vector<int> findLargestGroup(int nodeCount) {
    bool isCompleteGraph = true;

    for (int i = 0; i < nodeCount && isCompleteGraph; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            if (i != j && graph[i][j] != 1) {
                isCompleteGraph = false;
                break;
            }
        }
    }

    if (isCompleteGraph) {
        std::vector<bool> visited(MAX_NODES, false);
        std::vector<int> largestGroup;

        for (int i = 0; i < nodeCount; ++i) {
            if (!visited[i]) {
                std::vector<int> currentComponent;
                performDFS(i, visited, currentComponent);
                if (currentComponent.size() > largestGroup.size()) {
                    largestGroup = std::move(currentComponent);
                }
            }
        }
        return largestGroup;
    } else {
        return getLargestFullyConnectedGroup(nodeCount);
    }
}

int main() {
    int nodeCount;
    std::cin >> nodeCount;

    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            std::cin >> graph[i][j];
        }
    }

    std::vector<int> largestGroup = findLargestGroup(nodeCount);

    std::sort(largestGroup.begin(), largestGroup.end());
    for (size_t i = 0; i < largestGroup.size(); ++i) {
        std::cout << largestGroup[i];
        if (i < largestGroup.size() - 1) {
            std::cout << " ";
        }
    }

    return 0;
}
