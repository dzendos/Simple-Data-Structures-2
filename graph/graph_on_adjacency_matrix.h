#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "graph/graph.h"
#include "graph/edge.h"

#include "fibonacci_heap/fibonacci_heap.h"
#include "fibonacci_heap/fibonacci_heap_node.h"

/**
 * @brief The AdjacencyMatrix class implements graph using Adjacency matrix
 *
 * @author  Evgeny Gerasimov
 * @version 1.0; 19.04.2022
 * @see     Graph
 * @param   <V> the type of vertices
 * @param   <E> the type of edges
 */
template <typename V, typename E>
class AdjacencyMatrix : public Graph<V, E> {
private:
    /**
     * Worst case time complexity - O(n^2)
     *
     * @brief expandMatrix expands curent matrix by 2
     */
    void expandMatrix() {
        int newMaximumSize = maximumSize * 2;

        matrix.resize(newMaximumSize);
        edgeExist.resize(newMaximumSize);

        for (int i = 0; i < newMaximumSize; i++) {
            matrix[i].resize(newMaximumSize);
            edgeExist[i].resize(newMaximumSize);
            //isVertexInGraph.resize(newMaximumSize);
        }

        maximumSize = newMaximumSize;
    }

public:
    int maximumSize;
    int numberOfVertices;
    int lastIndex;

    std::vector<std::vector<E>>    matrix;
    std::vector<std::vector<bool>> edgeExist;

    std::unordered_map<V, int>  indexOfVertex;
    std::unordered_map<int, V>  vertexByIndex;

    std::vector<std::unordered_set<int>> neighbors;

    std::queue<int>   freeIndexes;

    AdjacencyMatrix() {
        maximumSize = 4;
        numberOfVertices = 0;
        lastIndex = 0;

        matrix.resize(maximumSize, std::vector<E>(maximumSize));
        edgeExist.resize(maximumSize, std::vector<bool>(maximumSize, false));
    }

    /**
     * Worst case time complexity - O(log(map.size))
     *
     * @brief vertexExist returns true if such vertex exists in the graph, false in another case
     * @param vertex to check
     * @return true if such vertex exists in the graph, false in another case
     */
    bool vertexExist(const V& vertex) {
        return indexOfVertex.count(vertex) == 1;
    }

    /**
     * Amortised time complexity - O(1)
     *
     * @brief insertVertex inserts vertex into graph
     * @param vertex - vertex to insert
     */
    virtual void insertVertex(const V& vertex) override {
        if (vertexExist(vertex))
            return; // such vertex already exists

        int insertIndex;

        if (!freeIndexes.empty()) {
            insertIndex = freeIndexes.front();
            freeIndexes.pop();
        } else {
            if (lastIndex == maximumSize)
                expandMatrix();

            neighbors.push_back(std::unordered_set<int>());

            insertIndex = lastIndex++;
        }

        numberOfVertices++;

        indexOfVertex[vertex] = insertIndex;
        vertexByIndex[insertIndex] = vertex;
    }

    /**
     * Worst case time complexity - O(log(map.size))
     *
     * @brief insertEdge inserts edge in a matrix
     * @param from initial node
     * @param to end node
     * @param edge edge that connects 2 nodes
     */
    virtual void insertEdge(const V& from, const V& to, const E& edge) override {
        if (!vertexExist(from) || !vertexExist(to))
            return; // at least one of the edge does not exist

        int fromIndex = indexOfVertex[from];
        int toIndex   = indexOfVertex[to];

        matrix[fromIndex][toIndex]    = edge;
        edgeExist[fromIndex][toIndex] = true;

        neighbors[fromIndex].insert(toIndex);
    }

    /**
     * Worst case time complexity - O(lastIndex)
     *
     * @brief removeVertex - removes vertex from matrix
     * @param vertex to remove
     */
    virtual void removeVertex(const V& vertex) override {
        if (!vertexExist(vertex))
            return; // vertex does not exist

        int index = indexOfVertex[vertex];

        for (int i = 0; i < lastIndex; i++)
            edgeExist[index][i] = edgeExist[i][index] = false;

        freeIndexes.push(index);

        indexOfVertex.erase(indexOfVertex.find(vertex));
        vertexByIndex.erase(vertexByIndex.find(index));
    }

    /**
     * Worst case time complexity - O(V)
     * Expected - O(1)
     *
     * @brief removeEdge removes directed edge given by 2 vertices
     *
     * @param from - beginning of the edge
     * @param to - end of the edge
     */
    virtual void removeEdge(const V& from, const V& to) override {
        if (!vertexExist(from) || !vertexExist(to))
            return;

        edgeExist[indexOfVertex[from]][indexOfVertex[to]] = false;

        neighbors[indexOfVertex[from]].erase(indexOfVertex[to]);
    }

    /**
     * Worst case time complexity - O(log(map.size))
     *
     * @brief areAdjacent checks whether 2 vertices are adjacent
     * @param vertex1 - first vertex
     * @param vertex2 - second vertex
     * @return true if vertices are adjacent and false in other cases
     */
    virtual bool areAdjacent(const V& vertex1, const V& vertex2) override {
        if (!vertexExist(vertex1) || !vertexExist(vertex2))
           return false;

        int index1 = indexOfVertex[vertex1];
        int index2 = indexOfVertex[vertex2];

        return edgeExist[index1][index2] || edgeExist[index2][index1];
    }

    /**
     * Worst case time complexity - O(V*log(matrix.size))
     *
     * @brief degree returns the degree of a given vertex
     * @param vertex - given vertex
     * @return the degree of a given vertex
     */
    virtual int degree(const V& vertex) override {
        if (indexOfVertex.count(vertex) == 0)
            return -1; // such vertex does not exist in the graph

        int index = indexOfVertex[vertex];

        int degree = 0;

        for (int i = 0; i < lastIndex; i++) {
            if (!vertexExist(i))
                continue;

            if (edgeExist[index][i])
                degree++;
        }

        return degree;
    }
};








