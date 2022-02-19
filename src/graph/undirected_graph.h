//
// Created by Antoni Solarski
// github.com/Antsol1000
//

#ifndef ALGORITHMS_UNDIRECRED_GRAPH_H
#define ALGORITHMS_UNDIRECRED_GRAPH_H

#include "helpers.h"

template<typename N, typename E>
class graph : public abstract_graph<N, E> {

    typedef typename abstract_graph<N, E>::node node;
    typedef typename abstract_graph<N, E>::edge edge;

    struct undirected_edge;

public:

    explicit graph(int n = 0) : abstract_graph<N, E>(n) {}

    std::string to_dot() override;

    void add_edge(int src, int dst, E e) override;

    bool is_acyclic() override;

    VVI strongly_connected_components() override;

    bool euler_path(VI& result) override;

private:

    void _is_acyclic_rec(bool &flag, bool *visited, int src);

    /**
     * Finds minimum spanning tree of the graph.
     * Uses Kruskal's algorithm in time O(E*logV).
     * @param g graph
     * @return cost of minimum spanning tree
     */
    template<typename V>
    friend int kruskal(const graph<V, weighted_edge> &g);

    /**
     * Finds minimum spanning tree of the graph.
     * Uses Prim's algorithm in time O(E*logV).
     * Unless using fibonacci heap then time complexity is O(E + V*logV).
     * @param g graph
     * @return cost of minimum spanning tree
     */
    template<typename V>
    friend int prim(const graph<V, weighted_edge> &g);

};



///IMPLEMENTATION*******************************************************************************************************

template<typename N, typename E>
struct graph<N, E>::undirected_edge : public abstract_graph<N, E>::edge {
    int rev;

    explicit undirected_edge(edge e) : edge(e), rev(-1) {}

    explicit undirected_edge(int dst, E e = E()) : edge(dst, e), rev(-1) {}
};

template<typename N, typename E>
void graph<N, E>::add_edge(int src, int dst, E e) {
    undirected_edge e1(dst, e);
    e1.rev = this->nodes[dst].size();
    undirected_edge e2(src, e);
    e2.rev = this->nodes[src].size();
    this->_add_edge(src, e1);
    this->_add_edge(dst, e2);
}

template<typename N, typename E>
VVI graph<N, E>::strongly_connected_components() {
    VVI result;
    bool *visited = new bool [this->size()];
    REP(i, this->size()) {
        if (!visited[i]) {
            VI list;
            this->_dfs_rec(i, visited, [&list](int x) { list.push_back(x); }, DUMMY_APPLIER(x), TRUE_PREDICATE(x));
            result.push_back(list);
        }
    }
    delete [] visited;
    return result;
}

template<typename N, typename E>
bool graph<N, E>::is_acyclic() {
    bool flag = true;
    bool *visited = new bool[this->size()];
    REP(i, this->size()) {
        if (!visited[i]) {
            this->_is_acyclic_rec(flag, visited, i);
        }
    }
    delete[] visited;
    return flag;
}

template<typename N, typename E>
void graph<N, E>::_is_acyclic_rec(bool &flag, bool *visited, int src) {
    if (!visited[src]) {
        visited[src] = true;
        for (edge ed: this->nodes[src]) {
            if (!visited[src] && ed.dst != src) {
                _is_acyclic_rec(flag, visited, ed.dst);
            }
        }
    } else {
        flag = false;
    }
}

template<typename N, typename E>
bool graph<N, E>::euler_path(VI& result) {
    int v = -1; // node to start
    QI l;
    int deg[this->size()]; // degree of node
    int of[this->size() + 1]; // prefixes sum of degrees, help to "remove" reverse undirected_edge
    REP(i, this->size()) {
        deg[i] = this->nodes[i].size();
        of[i + 1] = of[i] + deg[i];
        // if degree is odd then start with this node
        if (deg[i] % 2 == 1 || (v == -1 && deg[i] > 0))
            v = i;
    }

    bool *visited = new bool[of[this->size()]]; // used edges
    //dfs
    l.push_back(v);
    while (!l.empty()) {
        v = l.back();
        // is we used this undirected_edge omit
        while (deg[v] > 0 && visited[of[v] + deg[v] - 1]) {
            deg[v]--;
        }
        // if no edges, poll the node and push to result
        if (deg[v] == 0) {
            l.pop_back();
            result.push_back(v);
        } else {
            // get last undirected_edge, mark it as visited and go deeper
            undirected_edge ed = (undirected_edge) this->nodes[v][--deg[v]];
            visited[of[ed.dst] + ed.rev] = true;
            v = ed.dst;
            l.push_back(v);
        }
    }
    delete[] visited;
    // if we used all edges then true
    return 2 * (result.size() - 1) == of[this->size()];
}

template<typename N>
int kruskal(const graph<N, weighted_edge> &g) {
    VIN result;
    int cost = 0;
    VIE s;
    disjoint_sets<int> a;
    REP(i, g.size()) {
        a.add(i);
        for (auto e: g.nodes[i]) {
            s.push_back(MP(e.value, MP(i, e.dst)));
        }
    }
    SORT(s);

    REP(i, g.size() - 1) {
        int src = s[i].second.first, dst = s[i].second.second;
        if (a.find(src, dst)) {
            a.join(src, dst);
            result.push_back(s[i].second);
            cost += s[i].first;
        }
    }
    return cost;
}

template<typename N>
int prim(const graph<N, weighted_edge> &g) {
    VIN result;
    int cost = 0;
    bool visited[g.size()]{false};
    PQE q;
    visited[0] = true;
    for (auto e: g.nodes[0]) {
        q.push(MP(e.value, MP(0, e.dst)));
    }
    while (result.size() + 1 != g.size()) {
        auto ed = q.top();
        q.pop();
        int dst = ed.second.second;
        if (!visited[dst]) {
            visited[dst] = true;
            cost += ed.first;
            result.push_back(ed.second);
            for (auto e: g.nodes[dst]) {
                q.push(MP(e.value, MP(dst, e.dst)));
            }
        }
    }
    return cost;
}

template<typename N, typename E>
std::string graph<N, E>::to_dot() {
    std::string result = "graph { ";
    REP(i, this->size()) {
        FOR(j, this->nodes[i]) {
            if (i <= j->dst) {
                result += std::to_string(i) + " -- " + std::to_string(j->dst) + "; ";
            }
        }
    }
    result += "}";
    return result;
}


#endif //ALGORITHMS_UNDIRECRED_GRAPH_H
