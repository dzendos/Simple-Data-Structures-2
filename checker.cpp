#include <iostream>
#include <vector>
#include <climits>

#include "btree/btree.h"
#include "fibonacci_heap/fibonacci_heap.h"
#include "fibonacci_heap/fibonacci_heap_node.h"
#include "graph/graph_on_adjacency_matrix.h"

int main() {

    const std::vector<int> data = { -7, -5, -4, 0, -1, 3, 7, 3, 2, 1, -1, 5, 9, 15, 2, 4, 2, 0, 16, -2 };

    std::cout << "///////////////////" << std::endl <<
                 "/// BTREE CHECK ///" << std::endl <<
                 "///////////////////" << std::endl << std::endl;

    BTree<int, int*> tree(3);

    for (const int& w : data) {
        tree.add(w, new int(w));
        std::cout << "element: " << w <<
                     " value: " << *tree.lookup(w) <<
                     " does w+1 element exist: " << tree.contains(w+1) << std::endl;
    }

    std::cout << std::endl;

    // checking elements removing
    for (const int& w : data) {
        // tree.remove(w);
        auto elements = tree.lookupRange(INT_MIN, INT_MAX);
        for (const int* value : elements)
            std::cout << *value << " ";

        std::cout << std::endl;
    }

    std::cout << std::endl;

    std::cout << "////////////////////////////" << std::endl <<
                 "/// FIBONACCI HEAP CHECK ///" << std::endl <<
                 "////////////////////////////" << std::endl << std::endl;

    FibonacciHeap<int, int> heap(INT_MIN);

    int it = 1;

    std::vector<Node<int, int>*> nodes;

    for (const int& w : data) {
        nodes.push_back(new Node(w, w));
        heap.insert(nodes.back());

        std::cout << "min element after " << it++ << " insertion " << heap.findMin()->value << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < data.size(); i++) {
        heap.deleteItem(nodes[i]);

        if (i == data.size() - 1)
            continue;

        Node<int, int>* minNode = heap.findMin();
        if (minNode == nullptr) {
            std::cout << "heap is empty!" << std::endl;
            break;
        }

        std::cout << "min element after " << i+1 << " deletion " << minNode->value << std::endl;
    } 

    return 0;
}














