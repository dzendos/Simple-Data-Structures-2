#pragma once

#include <vector>

/**
 * @brief The RangeMap class implements interface for BTree
 *
 * @version 1.0; 02.04.2022
 * @param   <K> the type of key elements
 * @param   <V> the type of value elements
 */
template <typename K, typename V>
class RangeMap
{
public:
    int             size();                          // returns number of elements in range map
    bool            isEmpty();                       // checks whether range map is empty or nor
    void            add(const K&, const V&);         // inserts new item into map
    bool            contains(const K&);              // checks if a key is present
    V*              lookup(const K&);                // lookup a value by the key
    std::vector<V*> lookupRange(const K&, const K&); // lookup values for a range of keys

    void remove(const K&);                           // removes an item by key
};
