#pragma once

/**
 * @brief The Graph class implements interface for graph
 */
template <typename V, typename E>
class Graph {
public:
    virtual void insertVertex(const V&)                   = 0; // inserts vertex in graph
    virtual void insertEdge(const V&, const V&, const E&) = 0; // inserts edge in graph
    virtual void removeVertex(const V&)                   = 0; // removes vertex from graph
    virtual void removeEdge(const V&, const V&)           = 0; // removes edge from graph
    virtual bool areAdjacent(const V&, const V&)          = 0; // checks whether 2 vertices are adjacent or not
    virtual int  degree(const V&)                         = 0; // returns the degree of a given vertex
};
