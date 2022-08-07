#pragma once

#include "fibonacci_heap/fibonacci_heap.h"
#include "fibonacci_heap/fibonacci_heap_node.h"

/**
 * @brief The PriorityQueue class implements an interface for priority queue structures
 *
 * @version 1.0; 02.04.2022
 * @param   <K> the type of key elements
 * @param   <V> the type of value elements
 */
template <typename K, typename V>
class PriorityQueue
{
public:
    virtual void        insert(Node<K, V>* item);                       // insert node into heap
    virtual Node<K, V>* findMin();                                      // get node with minimum key
    virtual Node<K, V>* extractMin();                                   // extract mode with minimum key from heap
    virtual void        decreaseKey(Node<K, V>* item, const K& newKey); // decrease key of the node
    virtual void        deleteItem(Node<K, V>* item);                   // delete node
    virtual void        unionWith(PriorityQueue<K, V>& anotherQueue);   // unite 2 heaps
};
