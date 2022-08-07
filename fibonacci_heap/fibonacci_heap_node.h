#pragma once

/**
 * @brief The Node class implements a node structure for priority queue
 *
 * @author  Evgeny Gerasimov
 * @version 1.0; 12.04.2022
 * @see     PriorityQueue
 * @param   <K> the type of key elements
 * @param   <V> the type of value elements
 */
template <typename K, typename V>
class Node {
public:
    K   key;
    V   value;
    int degree;

    Node<K, V>* parent;
    Node<K, V>* left;
    Node<K, V>* right;
    Node<K, V>* child;

    Node(const K& key, const V& value) : key(key), value(value) {
        degree = 0;

        parent = child = left = right = nullptr;
    }
};
