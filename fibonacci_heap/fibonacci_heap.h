#pragma once

#include <vector>
#include <cmath>
#include <stdexcept>

#include "fibonacci_heap/fibonacci_heap_node.h"

/**
 * @brief The FibonacciHeap class implements a minimum priority queue structure
 *
 * @author  Evgeny Gerasimov
 * @version 1.0; 17.04.2022
 * @see     Node
 * @see     PriorityQueue
 * @param   <K> the type of key elements
 * @param   <V> the type of value elements
 */
template <typename K, typename V>
class FibonacciHeap {
private:
    Node<K, V>* minNode;
    size_t numberOfNodes;

    K minPossibleKey;

    /**
     * Worst case time complexity - O(1)
     *
     * @brief insertToRoot - inserts node to root list
     * @param node - node to insert
     */
    void insertToRoot(Node<K, V>* node) {
        if (minNode != nullptr) {
            node->left = minNode->left;
            node->right = minNode;
            minNode->left = node;
            node->left->right = node;
        }
        else {
            minNode = node;
            node->left = node->right = node;
        }
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief insertNode - inserts new node in the left of other node
     * @param node - shows at which level it is needed to insert
     * @param newNode - node to insert
     */
    void insertNode(Node<K, V>* node, Node<K, V>* newNode) {
        if (node == nullptr || newNode == nullptr)
            throw std::runtime_error("Merging unexisting nodes");

        newNode->left  = node->left;
        newNode->right = node;
        node->left = newNode;
        newNode->left->right = newNode;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief unlinkNode unlinks node from its level
     * @param node - node to unlink
     */
    void unlinkNode(Node<K, V>* node) {
        Node<K, V>* right = node->right;
        Node<K, V>* left  = node->left;

        right->left = left;
        left->right = right;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief unlinkChild unlinks first child of the parent from its level
     * @param parent - parent node
     */
    void unlinkChild(Node<K, V>* parent) {
        Node<K, V>* child = parent->child;

        if (child->right == child)
            parent->child = nullptr;
        else {
            parent->child = child->right;
            child->parent = nullptr;
            unlinkNode(child);
        }
    }

    /**
     * Amortized time complexity - O(log(n))
     *
     * @brief consolidate perform a merge of root trees into trees of different size
     */
    void consolidate() {
        int Dn = int(1 + log2(size()));

        std::vector<Node<K, V>*> degA(Dn, nullptr);

        Node<K, V>* current = minNode;

        do {
            int degree = current->degree;

            while (degA[degree] != nullptr && degA[degree] != current) {
                Node<K, V>* y = degA[degree];

                if (current->key > y->key)
                    std::swap(current, y);

                if (y == minNode)
                    minNode = current;

                moveChildToParent(y, current);

                degA[degree] = nullptr;
                degree++;
            }

            degA[degree] = current;
            current = current->right;

        } while (current != minNode);

        minNode = nullptr;

        for (int i = 0; i < Dn; i++) {
            if (degA[i] != nullptr) {
                Node<K, V>* prevMin = minNode;

                insertToRoot(degA[i]);

                if (prevMin != nullptr && degA[i]->key < minNode->key)
                    minNode = degA[i];
            }
        }
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief moveChildToParent connects node child with its new parent
     * @param child - child node
     * @param parent - parent node
     */
    void moveChildToParent(Node<K, V>* child, Node<K, V>* parent) {
        unlinkNode(child);

        child->parent = parent;
        if (parent->child != nullptr)
            insertNode(parent->child, child);
        else {
            parent->child = child;
            child->left = child->right = child;
        }

        parent->degree++;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief cut moves node to the root of the heap
     * @param node - node to move
     */
    void cut(Node<K, V>* node) {
        if (node->parent == nullptr) // we do not need to move into root
            return;

        unlinkNode(node); // unlinking it from its level list

        node->parent->degree--;

        if (node->parent->child == node) {          // if node is a first child
            if (node->right == node)                // if it is single one, then parent have no childs any more
                node->parent->child = nullptr;
            else                                    // else its first child becomes its second one
                node->parent->child = node->right;
        }

        // making node as a single tree
        node->right  = node;
        node->left   = node;
        node->parent = nullptr;

        // putting it into main forest (where minNode is)
        insertNode(minNode, node);
    }

public:
    FibonacciHeap(K minPossibleKey) : minPossibleKey(minPossibleKey) {
        minNode = nullptr;
        numberOfNodes = 0;
    }

    ~FibonacciHeap() {
        while (!isEmpty())
            extractMin();
    }

    /**
     * Amortized time complexity - O(1)
     * @brief insert inserts node into heap
     * @param item - node to insert
     */
    void insert(Node<K, V>* item) {
        insertToRoot(item);

        if (item->key < minNode->key)
            minNode = item;

        numberOfNodes++;
    }

    /**
     * @brief findMin returns minimum element of the heap
     * @return minimum element of the heap
     */
    Node<K, V>* findMin() {
        return minNode;
    }

    /**
     * Amortized time complexity
     *
     * @brief extractMin unlinks minimum element from heap and returns it (notice - user should free memory by its own)
     * @return extracted node
     */
    Node<K, V>* extractMin() {
        if (isEmpty())
            throw std::runtime_error("Extracting from empty heap");

        Node<K, V>* nodeToDelete = minNode;
        Node<K, V>* child        = nodeToDelete->child;

        while (child != nullptr) {
            unlinkChild(nodeToDelete);
            insertToRoot(child);
            child = nodeToDelete->child;
        }

        if (nodeToDelete->right == nodeToDelete) {
            minNode = nullptr;
        }
        else {
            minNode = nodeToDelete->right;

            unlinkNode(nodeToDelete);

            consolidate();
        }

        numberOfNodes--;

        return nodeToDelete;
    }

    /**
     * Amortized time complexity - O(1)
     *
     * @brief decreaseKey decreases the key of a node
     * @param item - node which key will be changed
     * @param newKey - new key to insert in node
     */
    void decreaseKey(Node<K, V> *item, const K &newKey) {
        if (item->parent != nullptr && newKey > item->parent->key) {
            item->key = newKey;
            return;
        }

        item->key = newKey;

        cut(item);

        minNode = item;
    }

    /**
     * Amortized time complexity - O(log(n))
     *
     * @brief deleteItem removes item from heap
     * @param item - node to delete
     */
    void deleteItem(Node<K, V> *item) {
        if (size() == 0)
            return;

        decreaseKey(item, minPossibleKey);
        extractMin();
    }

    /**
     * Amortized time complexity - O(1)
     *
     * @brief unionWith add another heap into current one
     * @param anotherQueue - heap to add
     */
    void unionWith(FibonacciHeap<K, V> &anotherQueue) {
        if (anotherQueue.size() == 0)
            return;

        if (size() == 0) {
            minNode       = anotherQueue.minNode;
            numberOfNodes = anotherQueue.size();
            return;
        }

        minNode->unionWith(anotherQueue.minNode);
        numberOfNodes += anotherQueue.size();

        if (anotherQueue.minNode < minNode)
            minNode = anotherQueue.minNode;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief size returns number of nodes in heap
     * @return number of nodes in heap
     */
    size_t size() {
        return numberOfNodes;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief isEmpty returns true if there are no nodes in heap, false in other case
     * @return true if there are no nodes in heap, false in other case
     */
    bool isEmpty() {
        return size() == 0;
    }
};
