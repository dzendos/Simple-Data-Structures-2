#pragma once

#include <vector>
#include <string>
#include <climits>

#include "graph/graph_on_adjacency_matrix.h"
#include "fibonacci_heap/fibonacci_heap.h"
#include "fibonacci_heap/fibonacci_heap_node.h"
#include "graph/edge.h"
#include "graph/graph_on_adjacency_matrix.h"

/**
 * Worst case time complexity - O(E + V*log(V))
 *
 * @brief GetMinSpanningForest uses Prim algorithm to put in the array "forest" the minnimum spanning forest og the given graph
 * @param graph - given graph
 * @param forest - resulting array
 */
template <typename V, typename E>
void GetMinSpanningForest(AdjacencyMatrix<V, E> &graph, std::vector<std::pair<std::string, std::string>>& forest) {
    std::unordered_set<int> isVisited;

    for (int i = 0; i < graph.lastIndex; i++) {
        if (isVisited.count(i) == 1)
            continue;

        isVisited.insert(i);

        FibonacciHeap<double, Edge> minEdges(-10000000);

        for (const int& vertex : graph.neighbors[i]) {
            if (isVisited.count(vertex) == 0)
                minEdges.insert(new Node(graph.matrix[i][vertex], Edge(i, vertex)));
        }

        while (!minEdges.isEmpty()) {
            auto* node = minEdges.extractMin();
            if (isVisited.count(node->value.vertex2)) {
                delete node;
                continue;
            }

            Edge edge = node->value;

            forest.push_back({ graph.vertexByIndex[edge.vertex1], graph.vertexByIndex[edge.vertex2] });

            isVisited.insert(edge.vertex2);

            for (const int& vertex : graph.neighbors[edge.vertex2]) {
                if (isVisited.count(vertex) == 0)
                    minEdges.insert(new Node(graph.matrix[edge.vertex2][vertex], Edge(edge.vertex2, vertex)));
            }

            delete node;
        }
    }
}











