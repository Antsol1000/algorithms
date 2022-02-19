//
// Created by Antoni Solarski
// github.com/Antsol1000
//

#ifndef ALGORITHMS_PRIORITY_QUEUE_H
#define ALGORITHMS_PRIORITY_QUEUE_H

template<typename T>
class priority_queue {

public:

    /**
     * check if queue is empty
     */
    virtual bool empty() const = 0;

    /**
     * @return number of items in queue
     */
    virtual size_t size() const = 0;

    /**
     * check if given key belongs to queue
     */
    virtual bool contains(const T &key) const = 0;

    /**
     * @return min key
     */
    virtual T top() const = 0;

    /**
     * insert new key to queue
     */
    virtual void push(const T &key) = 0;

    /**
     * extract min key from queue
     */
    virtual void pop() = 0;

    /**
     * decrease value of given key
     */
    virtual void decrease(const T &key, const T &new_key) = 0;

    /**
     * remove given key from queue
     */
    virtual void remove(const T &key) = 0;
};


#endif //ALGORITHMS_PRIORITY_QUEUE_H
