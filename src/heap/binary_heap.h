//
// Created by Antoni Solarski
// github.com/Antsol1000
//

#ifndef ALGORITHMS_BINARY_HEAP_H
#define ALGORITHMS_BINARY_HEAP_H

#include "priority_queue.h"
#include <unordered_map>

template<typename T>
class binary_heap : public priority_queue<T> {

public:

    /**
     * creates empty heap
     */
    binary_heap();

    /**
     * creates heap from given items
     * time complexity: O(n)
     */
    binary_heap(std::initializer_list<T> items);

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
     * @return min key
     * time complexity: O(1)
     */
    T top() const override;

    /**
     * insert new key to heap
     * time complexity: O(log n)
     */
    void push(const T &key) override;

    /**
     * extract min key from heap
     * time complexity: O(log n)
     */
    void pop() override;

    /**
     * decrease value of given key
     * time complexity: O(log n)
     */
    void decrease(const T &key, const T &new_key) override;

    /**
     * remove given key from heap
     * time complexity: O(log n)
     */
    void remove(const T &key) override;

private:
    typedef std::vector<T> V;
    typedef std::unordered_map<T, int> MAP;
    V items;
    MAP node_map;
    void _heapify(int i);
    void _swap(int i, int j);
    void _push_top(int i);
};

template<typename T>
binary_heap<T>::binary_heap() : items(), node_map() {

}

template<typename T>
binary_heap<T>::binary_heap(std::initializer_list<T> items) : items(items), node_map() {
    int i = 0;
    for (auto item: items) {
        node_map[item] = i++;
    }
    for (i = (items.size() - 2) / 2; i > -1; --i) {
        _heapify(i);
    }
}

template<typename T>
void binary_heap<T>::_heapify(int i) {
    int min = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < items.size()) {
        min = items[min] > items[l] ? l : min;
    }
    if (r < items.size()) {
        min = items[min] > items[r] ? r : min;
    }
    if (min != i) {
        _swap(min, i);
        _heapify(min);
    }
}

template<typename T>
void binary_heap<T>::_swap(int i, int j) {
    T temp = items[i];
    items[i] = items[j];
    items[j] = temp;
    node_map[items[j]] = j;
    node_map[items[i]] = i;
}

template<typename T>
bool binary_heap<T>::empty() const {
    return items.empty();
}

template<typename T>
size_t binary_heap<T>::size() const {
    return items.size();
}

template<typename T>
bool binary_heap<T>::contains(const T &key) const {
    auto ptr = node_map.find(key);
    return ptr != node_map.end();
}

template<typename T>
T binary_heap<T>::top() const {
    return items.front();
}

template<typename T>
void binary_heap<T>::push(const T &key) {
    items.push_back(key);
    node_map[key] = items.size() - 1;
    _push_top(items.size() - 1);
}

template<typename T>
void binary_heap<T>::_push_top(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (items[parent] > items[i]) {
            _swap(parent, i);
        } else {
            break;
        }
        i = parent;
    }
}

template<typename T>
void binary_heap<T>::pop() {
    _swap(0, items.size() - 1);
    items.pop_back();
    _heapify(0);
}

template<typename T>
void binary_heap<T>::decrease(const T &key, const T &new_key) {
    if (key < new_key) {
        throw "cannot increase key";
    }
    int i = node_map[key];
    items[i] = new_key;
    node_map.erase(key);
    node_map[new_key] = i;
    _push_top(i);
}

template<typename T>
void binary_heap<T>::remove(const T &key) {
    int i = node_map[key];
    _swap(i, items.size() - 1);
    items.pop_back();
    node_map.erase(key);
    _heapify(i);
}


#endif //ALGORITHMS_BINARY_HEAP_H
