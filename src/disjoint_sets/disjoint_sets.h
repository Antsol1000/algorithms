//
// Created by Antoni Solarski
// github.com/Antsol1000
//

#ifndef ALGORITHMS_DISJOINT_SETS_H
#define ALGORITHMS_DISJOINT_SETS_H


#include <unordered_map>

template<typename T>
class disjoint_sets {

private:

    struct Node;

    std::unordered_map<T, Node*> dict;

    Node* find(Node* node);
    void join(Node* x, Node* y);

public:

    disjoint_sets() : dict() {}
    disjoint_sets(std::initializer_list<T> keys);
    ~disjoint_sets();

    void add(T key);
    bool find(T x, T y);
    void join(T x, T y);

};

template<typename T>
disjoint_sets<T>::disjoint_sets(std::initializer_list<T> keys) : disjoint_sets() {
    for (T key : keys) {
        add(key);
    }
}

template<typename T>
disjoint_sets<T>::~disjoint_sets() {
    for (std::pair<T, Node*> x : dict) {
        delete x.second;
    }
    dict.clear();
}

template<typename T>
void disjoint_sets<T>::add(T key) {
    Node* n = new Node(key);
    auto x = dict.insert(std::make_pair(key, n));
    if (!x.second) {
        delete n;
    }
}

template<typename T>
bool disjoint_sets<T>::find(T x, T y) {
    return find(dict[x])->key == find(dict[y])->key;
}

template<typename T>
void disjoint_sets<T>::join(T x, T y) {
    join(dict[x], dict[y]);
}

template<typename T>
typename disjoint_sets<T>::Node *disjoint_sets<T>::find(disjoint_sets::Node *node) {
    if (node->parent == nullptr) {
        return node;
    }
    node->parent = find(node->parent);
    return node->parent;
}

template<typename T>
void disjoint_sets<T>::join(disjoint_sets::Node *x, disjoint_sets::Node *y) {
    Node *xr = find(x);
    Node *yr = find(y);
    if (xr == yr) {
        return;
    } else if (yr->rank > xr->rank) {
        xr->parent = yr;
    } else if (xr->rank > yr->rank) {
        yr->parent = xr;
    } else {
        yr->parent = xr;
        xr->rank++;
    }
}

template<typename T>
struct disjoint_sets<T>::Node {
    T key;
    Node *parent;
    int rank;
    explicit Node(T key) : key(key), parent(nullptr), rank(0) {}
};


#endif //ALGORITHMS_DISJOINT_SETS_H
