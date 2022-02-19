//
// Created by Antoni Solarski
// github.com/Antsol1000
//

#ifndef ALGORITHMS_FIBONACCI_HEAP_H
#define ALGORITHMS_FIBONACCI_HEAP_H

#include <unordered_map>
#include <queue>
#include <cmath>
#include "priority_queue.h"

template<typename T>
class fibonacci_heap : public priority_queue<T> {

public:

    /**
     * creates empty heap
     */
    fibonacci_heap();

    /**
     * check if heap is empty
     * time complexity: O(1)
     */
    bool empty() const override;

    /**
     * @return number of items in heap
     * time complexity: O(1)
     */
    size_t size() const override;

    /**
     * check if given key belongs to heap
     * time complexity: O(1)
     */
    bool contains(const T &key) const override;

    /**
     * join two fibonacci heaps
     * time complexity: O(1)
     */
    void merge(fibonacci_heap<T> &h);

    /**
     * @return min key
     * time complexity: O(1)
     */
    T top() const override;

    /**
     * insert new key to heap
     * time complexity: O(1)
     */
    void push(const T &key) override;

    /**
     * extract min key from heap
     * time complexity: O(log n)
     */
    void pop() override;

    /**
     * decrease value of given key
     * time complexity: O(1)
     */
    void decrease(const T &key, const T &new_key) override;

    /**
     * remove given key from heap
     * time complexity: O(log n)
     */
    void remove(const T &key) override;

private:
    struct node;
    typedef std::queue<node *> Q;
    typedef std::unordered_map<T, node *> MAP;
    MAP node_map;
    node *root;
    size_t n;
    Q _get_all_siblings(node *x);
    void _add_to_roots(node *x);
    void _check_for_root(node *x);
    void _remove_from_roots(node *x);
    void _consolidate();
    void _swap(node *&x, node *&y);
    void _link(node *y, node *x);
    void _add_to_children(node *p, node *c);
    void _cut(node *x, node *y);
    void _remove_from_children(node *p, node *c);
    void _cut_cascade(node *x);
};

template<typename T>
fibonacci_heap<T>::fibonacci_heap() : n(0), node_map(), root(nullptr) {

}

template<typename T>
struct fibonacci_heap<T>::node {
    T key;
    node *p, *c, *l, *r;
    int deg;
    bool mark;

    explicit node(const T &key) : key(key), p(nullptr), c(nullptr), l(this), r(this), deg(0), mark(false) {}
};


template<typename T>
bool fibonacci_heap<T>::empty() const {
    return n == 0;
}

template<typename T>
size_t fibonacci_heap<T>::size() const {
    return n;
}

template<typename T>
bool fibonacci_heap<T>::contains(const T &key) const {
    auto ptr = node_map.find(key);
    return ptr != node_map.end();
}

template<typename T>
void fibonacci_heap<T>::merge(fibonacci_heap &h) {
    if (h.root == nullptr) return;
    node *temp_t = root->r;
    node *temp_h = h.root->l;
    root->r = h.root;
    h.root->l = root;
    temp_t->l = temp_h;
    temp_h->r = temp_t;
    _check_for_root(h.root);
    this->n += h.n;
    for (auto item : h.node_map) {
        node_map[item.first] = item.second;
    }
}

template<typename T>
T fibonacci_heap<T>::top() const {
    return root->key;
}

template<typename T>
void fibonacci_heap<T>::push(const T &key) {
    node *x = new node(key);
    node_map[key] = x;
    _add_to_roots(x);
    _check_for_root(x);
    n++;
}

template<typename T>
void fibonacci_heap<T>::_add_to_roots(fibonacci_heap::node *x) {
    if (root == nullptr) {
        root = x;
    } else {
        node *temp = root->r;
        root->r = x;
        temp->l = x;
        x->l = root;
        x->r = temp;
    }
}

template<typename T>
void fibonacci_heap<T>::_check_for_root(fibonacci_heap::node *x) {
    if (root->key > x->key) {
        root = x;
    }
}

template<typename T>
void fibonacci_heap<T>::pop() {
    node *z = root;
    if (z->c != nullptr) {
        Q q = _get_all_siblings(z->c);
        while (!q.empty()) {
            node *c = q.front();
            q.pop();
            _add_to_roots(c);
            c->p = nullptr;
        }
    }
    _remove_from_roots(z);
    if (z == z->r) {
        root = nullptr;
    } else {
        root = z->r;
        _consolidate();
    }
    n--;
    node_map.erase(z->key);
    delete z;
}

template<typename T>
typename fibonacci_heap<T>::Q fibonacci_heap<T>::_get_all_siblings(fibonacci_heap::node *x) {
    Q q;
    node *y = x;
    do {
        q.push(y);
        y = y->r;
    } while (y != x);
    return q;
}

template<typename T>
void fibonacci_heap<T>::_remove_from_roots(fibonacci_heap::node *x) {
    node *temp_l = x->l;
    node *temp_r = x->r;
    temp_l->r = temp_r;
    temp_r->l = temp_l;
}

template<typename T>
void fibonacci_heap<T>::_consolidate() {
    int max_deg = std::ceil(std::log2(n) + 1);
    node **A = new node *[max_deg];
    for (int i = 0; i < max_deg; i++) {
        A[i] = nullptr;
    }
    Q q = _get_all_siblings(root);
    while (!q.empty()) {
        node *x = q.front();
        q.pop();
        int d = x->deg;
        while (A[d] != nullptr) {
            node *y = A[d];
            if (x->key > y->key) {
                _swap(x, y);
            }
            _link(y, x);
            A[d] = nullptr;
            d = d + 1;
        }
        A[d] = x;
    }

    root = nullptr;
    for (int i = 0; i < max_deg; i++) {
        if (A[i] != nullptr) {
            if (root == nullptr) {
                root = A[i];
                root->l = root;
                root->r = root;
                root->p = nullptr;
            } else {
                _add_to_roots(A[i]);
                _check_for_root(A[i]);
            }
        }
    }
}

template<typename T>
void fibonacci_heap<T>::_swap(fibonacci_heap::node *&x, fibonacci_heap::node *&y) {
    node *temp = x;
    x = y;
    y = temp;
}

template<typename T>
void fibonacci_heap<T>::_link(fibonacci_heap::node *y, fibonacci_heap::node *x) {
    _remove_from_roots(y);
    _add_to_children(x, y);
    y->mark = false;
}

template<typename T>
void fibonacci_heap<T>::_add_to_children(fibonacci_heap::node *p, fibonacci_heap::node *c) {
    p->deg++;
    c->p = p;
    if (p->c == nullptr) {
        p->c = c;
        c->l = c;
        c->r = c;
    } else {
        node *temp = p->c->r;
        p->c->r = c;
        temp->l = c;
        c->l = p->c;
        c->r = temp;
    }
}

template<typename T>
void fibonacci_heap<T>::decrease(const T &key, const T &new_key) {
    if (key < new_key) {
        throw "cannot increase key";
    }
    node *x = node_map[key];
    node_map.erase(key);
    x->key = new_key;
    node_map[new_key] = x;
    node *y = x->p;
    if (y != nullptr && x->key < y->key) {
        _cut(x ,y);
        _cut_cascade(y);
    }
    _check_for_root(x);
}

template<typename T>
void fibonacci_heap<T>::_cut(fibonacci_heap::node *x, fibonacci_heap::node *y) {
    _remove_from_children(y, x);
    _add_to_roots(x);
    x->mark = false;
}

template<typename T>
void fibonacci_heap<T>::_remove_from_children(node *p, node *c) {
    node *temp_l = c->l;
    node *temp_r = c->r;
    temp_l->r = temp_r;
    temp_r->l = temp_l;
    c->p = nullptr;
    if (p->c == c) {
        p->c = temp_l;
    }
    if (c->l == c) {
        p->c = nullptr;
    }
}

template<typename T>
void fibonacci_heap<T>::_cut_cascade(fibonacci_heap::node *x) {
    node *z = x->p;
    if (z != nullptr) {
        if (x->mark == false) {
            x->mark = true;
        } else {
            _cut(x, z);
            _cut_cascade(z);
        }
    }
}

template<typename T>
void fibonacci_heap<T>::remove(const T &key) {
    decrease(key, root->key - 1);
    pop();
}


#endif //ALGORITHMS_FIBONACCI_HEAP_H
