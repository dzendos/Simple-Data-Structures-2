#pragma once

#include <vector>

/**
 * Worst time complexity - O(right - left) (linear)
 *
 * @brief merge - merge two parts of a vector together
 * @param vector - vector that contains elements
 * @param left - beginning of the first array
 * @param mid - end of the first and beggining of the second array
 * @param right - end of the second array
 */
template <typename T>
void merge(std::vector<T>& vector, size_t left, size_t mid, size_t right) {
    std::vector<T> mergedVector(right - left);

    size_t leftIterator  = left;
    size_t rightIterator = mid;

    while (leftIterator != mid || rightIterator != right) {
        size_t position = (leftIterator + rightIterator) - (left + mid); // position to insert in merged array

        if (leftIterator == mid)                               mergedVector[position] = vector[rightIterator++];
        else if (rightIterator == right)                       mergedVector[position] = vector[leftIterator++];
        else if (vector[rightIterator] < vector[leftIterator]) mergedVector[position] = vector[rightIterator++];
        else                                                   mergedVector[position] = vector[leftIterator++];
    }

    for (size_t i = left; i != right; i++)
        vector[i] = mergedVector[i - left];
}

/**
 * Worst time complexity - O(n * log(n), where n - size of a vector
 *
 * @brief mergeSort - merge sort of a vector
 * @param vector - vector to sort
 */
template <typename T>
void mergeSort(std::vector<T>& vector) {
    for (size_t i = 1; i < vector.size(); i *= 2)
        for (size_t j = 0; j < vector.size() - i; j += 2 * i)
            merge(vector, j, j + i, min(j + 2 * i, vector.size()));
}
