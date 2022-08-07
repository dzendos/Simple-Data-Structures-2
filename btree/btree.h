#pragma once

#include "btree/range_map.h"
#include "btree/btree_node.h"

/**
 * @brief The BTree class implements range map
 *
 * @author  Evgeny Gerasimov
 * @version 1.0; 05.04.2022
 * @see     RangeMap
 * @param   <K> the type of key elements
 * @param   <V> the type of value elements
 */
template <typename K, typename V>
class BTree : public RangeMap<K, V>
{
private:
    BTreeNode<K, V>* root;
    int length;
    int t;

public:
    BTree(int t) : t(t), length(0), root(nullptr) { }

    int             size();
    bool            isEmpty();

    void            add(const K&, const V&);

    bool            contains(const K& key);
    V               lookup(const K& key);
    std::vector<V>  lookupRange(const K& from, const K& to);
};

/**
 * Worst case time complexity - O(1)
 *
 * @brief size - returns number of elements in BTree
 * @return number of elements in BTree.
 */
template<typename K, typename V>
int BTree<K, V>::size() {
    return length;
}

/**
 * Worst case time complexity - O(1)
 *
 * @brief isEmpty - returns true if BTree is empty, false otherwise
 * @return true if BTree is empty, false otherwise.
 */
template<typename K, typename V>
bool BTree<K, V>::isEmpty() {
    return length == 0;
}


/**
 * Worst case time complexity - O(t * h)
 *
 * @brief add - inserts new element into binary tree by the given key
 * @param key - key of the element
 * @param value - value of the element
 */
template<typename K, typename V>
void BTree<K, V>::add(const K& key, const V& value) {
    if (root == nullptr) {
        root = new BTreeNode<K, V>(t, true);
        root->insertInNode(key, value);

        return;
    }

    if (root->isFull(t)) {
        BTreeNode<K, V>* new_root = new BTreeNode<K, V>(t, false);

        new_root->childs.push_back(root);

        new_root->splitChild(root, 0);

        root = new_root;
    }

    root->insert(key, value, t);

    length++;
}

/**
 * Worst case time complexity - O(t * log(BTree.length))
 *
 * @brief contains - check whether the element with the given key exists in the tree
 * @param key to find
 * @return true if element with the given key exist, false in other case.
 */
template<typename K, typename V>
bool BTree<K, V>::contains(const K& key) {
    return lookup(key) != nullptr;
}

/**
 * Worst case time complexity - O(t * log(BTree.length))
 *
 * @brief lookup - returns the value of an element with the given key if it exists, nullptr otherwise
 * @param key of an element
 * @return the value of an element with the given key if it exists, nullptr otherwise.
 */
template<typename K, typename V>
V BTree<K, V>::lookup(const K& key) {
    std::vector<V> element;

    if (root != nullptr) {
        const bool is_single = true;

        root->search(key, key, element, is_single);
    }

    return element.size() == 0 ? nullptr : element[0];
}

/**
 * Worst case time complexity - O(BTree.length)
 *
 * @brief lookupRange returns an unsorted set of elements with the keys belonging to the given range
 * @param from - from which key we start
 * @param to - in what key we stop
 * @return an unsorted set of elements with the keys belonging to the given range.
 */
template<typename K, typename V>
std::vector<V> BTree<K, V>::lookupRange(const K& from, const K& to) {
    std::vector<V> range;

    if (root != nullptr) {
        const bool is_single = false;

        root->search(from, to, range, is_single);
    }

    return range;
}
