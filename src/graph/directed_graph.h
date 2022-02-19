//
// Created by Antoni Solarski
// github.com/Antsol1000
//

#ifndef ALGORITHMS_DIRECTED_GRAPH_H
#define ALGORITHMS_DIRECTED_GRAPH_H

#include "helpers.h"


template<typename N, typename E>
class digraph : public abstract_graph<N, E> {

    typedef typename abstract_graph<N, E>::node node;
    typedef typename abstract_graph<N, E>::edge edge;

public:

    explicit digraph(int n = 0) : abstract_graph<N, E>(n) {}

    std::string to_dot() override;

    void add_edge(int src, int dst, E e = E()) override;

    bool is_acyclic() override;

    VVI strongly_connected_components() override;

    bool euler_path(VI &result) override;

    /**
     * @return transposed graph
     */
    digraph<N, E> transpose();

    /**
     * Sort nodes in topological order. Uses DFS.
     * Time complexity is O(V + E)
     * @return
     */
    VI topological_sort();

    /**
     * Sort nodes in topological order. Uses Kahn's algorithm.
     * Time complexity is O(V + E)
     * @return
     */
    VI kahn_topological_sort();

    /**
     * @return vector with in degree of each node
     */
    VI get_in_degrees();

private:

    /**
     * Calculates maximum flow of given network, puts proper flows inside edges.
     * Uses Edmonds-Karp's algorithm in time O(V*E^2)
     * @param g graph
     * @param s source
     * @param t sink
     * @return maximum flow
     */
    template<typename V>
    friend int edmonds_karp(digraph<V, flowable> &g, int s, int t);

    /**
     * Calculates maximum flow of given network, puts proper flows inside edges.
     * Uses Dinic's algorithm in time O(E*V^2)
     * @param g graph
     * @param s source
     * @param t sink
     * @return maximum flow
     */
    template<typename V>
    friend int dinic(digraph<V, flowable> &g, int s, int t);

};



///IMPLEMENTATION*******************************************************************************************************

template<typename N, typename E>
void digraph<N, E>::add_edge(int src, int dst, E e) {
    _add_edge(src, dst, e);
}

template<typename N, typename E>
bool digraph<N, E>::is_acyclic() {
    VI top = topological_sort();
    int times[this->size()];
    int time = 0;
    for (int i: top) {
        times[i] = time++;
    }
    REP(i, this->size()) {
        for (edge ed: this->nodes[i]) {
            if (times[i] >= times[ed.dst]) {
                return false;
            }
        }
    }
    return true;
}

template<typename N, typename E>
digraph<N, E> digraph<N, E>::transpose() {
    digraph<N, E> transposed;
    for (node n: this->nodes) {
        transposed.add_node(n);
    }
    REP(i, this->size()) {
        for (edge e: this->nodes[i]) {
            transposed.add_edge(i, e.dst, e);
        }
    }
    return transposed;
}

template<typename N, typename E>
VI digraph<N, E>::topological_sort() {
    VI list;
    bool visited[this->size()]{false};
    REP(i, this->size()) {
        if (!visited[i]) {
            this->_dfs_rec(i, visited, [](int x) {}, [&list](int x) { list.push_back(x); });
        }
    }
    return list;
}

template<typename N, typename E>
VI digraph<N, E>::get_in_degrees() {
    VI result(this->size(), 0);
    REP(i, this->size()) {
        for (edge e: this->nodes[i]) {
            result[e.dst]++;
        }
    }
    return result;
}

template<typename N, typename E>
VI digraph<N, E>::kahn_topological_sort() {
    VI result;
    VI in_degrees = get_in_degrees();

    // push nodes without predecessors to queue
    QI q;
    REP(i, this->size()) {
        if (in_degrees[i] == 0) {
            q.push_back(i);
        }
    }

    // poll node from queue, push it to result and "remove" it edges
    // if some node lost all predecessors then push it to queue
    while (!q.empty()) {
        int v = q.front();
        q.pop_front();
        result.push_back(v);
        for (edge e: this->nodes[v]) {
            in_degrees[e.dst]--;
            if (in_degrees[e.dst] == 0) {
                q.push_back(e.dst);
            }
        }
    }

    return result;
}

template<typename N, typename E>
bool digraph<N, E>::euler_path(VI &result) {
    VI in_degrees = get_in_degrees();

    int edges = 0; // total number of edges
    int v = -1; // index of node to start path (or cycle)
    REP(i, this->size()) {
        // if out degree is greater than in degree start from this node
        if (this->nodes[i].size() > in_degrees[i])
            v = i;
            // if out degree is positive and you have nothing to start with, start from this node
        else if (this->nodes[i].size() > 0 && v == -1)
            v = i;
        // count edges
        edges += this->nodes[i].size();
    }

    // dfs
    QI l;
    l.push_back(v);
    while (!l.empty()) {
        // if no outgoing undirected_edge, push node to result
        // and go back
        if (in_degrees[l.back()] == 0) {
            v = l.back();
            l.pop_back();
            result.push_back(v);
        } // else remove undirected_edge and go deeper
        else {
            v = this->nodes[v][--in_degrees[v]].dst;
            l.push_back(v);
        }
    }

    REV(result);

    // if all edges were used then true
    return result.size() == (edges + 1);
}

template<typename N, typename E>
VVI digraph<N, E>::strongly_connected_components() {
    int components = 0;
    VVI result;
    VI temp;
    bool visited[this->size()]{false};
    REP(i, this->size()) {
        if (!visited[i]) {
            this->_dfs_rec(i, visited, [&temp](int x) { temp.push_back(x); }, [](int x) {});
        }
    }

    digraph<N, E> transposed = transpose();
    REP(i, this->size()) {
        visited[i] = false;
    }
    for (int i = this->size() - 1; i >= 0; --i) {
        int v = temp[i];
        if (!visited[v]) {
            VI tt;
            result.push_back(tt);
            components++;
            int final_components = components;
            this->_rec_dfs(visited, v,
                           [&result, &final_components](int x) { result[final_components].push_back(x); },
                           [](int x) {});
        }
    }

    return result;
}

template<typename N, typename E>
std::string digraph<N, E>::to_dot() {
    std::string result = "digraph { ";
    REP(i, this->size()) {
        FOR(j, this->nodes[i]) {
            result += std::to_string(i) + " -> " + std::to_string(j->dst) + "; ";
        }
    }
    result += "}";
    return result;
}


#endif //ALGORITHMS_DIRECTED_GRAPH_H
