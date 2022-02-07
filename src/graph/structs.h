//
// Created by Antoni Solarski
// github.com/Antsol1000
//


#ifndef ALGORITHMS_STRUCTS_H
#define ALGORITHMS_STRUCTS_H

struct empty {
};

struct weighted_edge {
    int value;

    explicit weighted_edge(int value = 0) : value(value) {}
};

struct flowable {
    int cap, cost, flow;

    explicit flowable(int cap, int flow = 0, int cost = 0) : cap(cap), flow(flow), cost(cost) {}
};

struct tree_node {
    int parent, level;
};

struct timed_node {
    int time_in, time_out;
};

#endif //ALGORITHMS_STRUCTS_H
