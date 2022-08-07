#pragma once

#include <vector>
#include <algorithm>

/**
 * @brief The BTreeNode class implements node for BTree
 *
 * @author  Evgeny Gerasimov
 * @version 1.0; 05.04.2022
 * @see     BTree
 * @param   <K> the type of key elements
 * @param   <V> the type of value elements
 */
template <typename K, typename V>
class BTreeNode
{
private:
    template <typename KEY, typename VALUE>
    friend class BTree;

protected:
    std::vector<K>         keys;
    std::vector<V>         values;
    std::vector<BTreeNode*> childs;

    bool is_leaf;

    void search(const K& from, const K& to,
                std::vector<V>& range,
                const bool& first_el_only);

    size_t insertInNode(const K& key, const V& value);

    void   splitChild(BTreeNode* child, int index);

    size_t size();

    bool   isFull(size_t t);

    void   insert(const K& key, const V& value, const int& t);

public:
    BTreeNode(int t, bool is_leaf = false) {
        keys.reserve(2*t);
        values.reserve(2*t);
        childs.reserve(2*t);

        this->is_leaf = is_leaf;
    }
};

/**
 * Worst case time complexity - O(BTree.length)
 *
 * @brief search returns an unsorted set of elements with the keys belonging to the given range
 * @param from - minimal value
 * @param to - maximum value
 * @param range - resulting vector of values
 * @param first_el_only - is needed if we want to find only first element in a given range
 */
template<typename K, typename V>
void BTreeNode<K, V>::search(const K& from, const K& to,
                             std::vector<V> &range,
                             const bool& first_el_only)
{
    // if we find for only 1 element and already found it - return
    if (first_el_only && range.size() != 0)
        return;

    int it;
    for (it = 0; it < keys.size(); it++) {
        // if key is to small - take next
        if (keys[it] < from)
            continue;

        // else if also key is less or equal than "to"
        if (keys[it] <= to) {
            if (first_el_only && range.size() != 0)
                return;

            range.push_back(values[it]); // put in the range

            // if not a leaf - go left
            if (!is_leaf)
                childs[it]->search(from, to, range, first_el_only);
        }

        // if keys became too big - exit
        if (keys[it] > to)
            break;
    }

    // if not a leaf - go right from the last passed element
    if (!is_leaf)
        childs[it]->search(from, to, range, first_el_only);
}

/**
 * Worst case time complexity - O(t), where t is a minimum degree of a B-Tree
 *
 * @brief insert returns index of an inserted element
 * @param key - key to insert
 * @param value - value of the element
 * @return index of inserted element
 */
template<typename K, typename V>
size_t BTreeNode<K, V>::insertInNode(const K& key, const V& value) {
    if (!is_leaf)
        return -1;

    typename std::vector<K>::iterator it = keys.insert(std::upper_bound(keys.cbegin(), keys.cend(), key), key);
    values.insert(values.cbegin() + (it - keys.cbegin()), value);

    return it - keys.cbegin();
}

/**
 * Worst case time complexity - O(t)
 *
 * @brief splitChild split a given child into to if it is full
 * @param child - child to split
 * @param index - index of a child in "childs" array of a parent node
 */
template<typename K, typename V>
void BTreeNode<K, V>::splitChild(BTreeNode *child, int index) {
    int t = (child->size() + 1) / 2;

    BTreeNode<K, V>* new_right_node = new BTreeNode<K, V>(t, child->is_leaf);

    size_t mid = child->size() / 2;

    // changing given node
    keys.insert(keys.cbegin() + index, child->keys[mid]);
    values.insert(values.cbegin() + index, child->values[mid]);
    childs.insert(childs.cbegin() + index + 1, new_right_node);

    // splitting keys and values between 2 nodes
    std::vector<K> left_child_keys(child->keys.cbegin(), child->keys.cbegin() + mid);
    std::vector<V> left_child_values(child->values.cbegin(), child->values.cbegin() + mid);

    std::vector<K> right_child_keys(child->keys.cbegin() + mid + 1, child->keys.cend());
    std::vector<V> right_child_values(child->values.cbegin() + mid + 1, child->values.cend());

    // assigning new keys and values
    child->keys = left_child_keys;
    child->values = left_child_values;

    new_right_node->keys = right_child_keys;
    new_right_node->values = right_child_values;

    // if not a leaf - splitting also childs
    if (!child->is_leaf) {
        std::vector<BTreeNode*> left_child_childs(child->childs.cbegin(), child->childs.cbegin() + mid + 1);
        std::vector<BTreeNode*> right_child_childs(child->childs.cbegin() + mid + 1, child->childs.cend());

        child->childs          = left_child_childs;
        new_right_node->childs = right_child_childs;
    }
}

/**
 * Worst case time complexity - O(1)
 *
 * @brief size returns number of elements in the node
 * @return number of elements in the node.
 */
template<typename K, typename V>
size_t BTreeNode<K, V>::size() {
    return keys.size();
}

/**
 * Worst case time complexity - O(1)
 *
 * @brief isFull - returns true if node is full, false otherwise
 * @param t - minimum degree of a node
 * @return true if node is full, false otherwise.
 */
template<typename K, typename V>
bool BTreeNode<K, V>::isFull(size_t t) {
    return size() == 2*t - 1;
}

/**
 * Worst case time complexity - O(t * h), where h - height of the tree
 *
 * @brief insert - inserts an element with the given key and value into non full node
 * @param key - key to insert
 * @param value - value of the element
 */
template<typename K, typename V>
void BTreeNode<K, V>::insert(const K& key, const V& value, const int& t) {
    if (is_leaf) {
        insertInNode(key, value);
        return;
    }

    int index_of_a_child = 0;

    // finding in which leaf is needed to insert
    while (index_of_a_child < size() && key > keys[index_of_a_child])
        index_of_a_child++;

    if (childs[index_of_a_child]->isFull(t)) {
        splitChild(childs[index_of_a_child], index_of_a_child);

        if (key > keys[index_of_a_child]) // in this index has pushed element from the child node
            index_of_a_child++;
    }

    childs[index_of_a_child]->insert(key, value, t);
}

















