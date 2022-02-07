//
// Created by Antoni Solarski
// github.com/Antsol1000
//

#ifndef ALGORITHMS_ABSTRACT_GRAPH_H
#define ALGORITHMS_ABSTRACT_GRAPH_H

#include "helpers.h"

template<typename N, typename E>
class abstract_graph {

protected:

    struct edge;
    struct node;

    std::vector<node> nodes;

public:

    explicit abstract_graph(int n = 0) : nodes(n) {}

    /**
     * @return number of nodes in graph
     */
    size_t size() const;

    /**
     * prints to standard output adjacency list of graph
     */
    void print();

    /**
     * @return string containing representation of graph in dot language format for graphviz
     */
    virtual std::string to_dot() = 0;

    /**
     * creates new node
     * @param n value to insert into node
     */
    void add_node(N n = N());

    /**
     * adds new edge from node src to node dst and value e
     */
    virtual void add_edge(int src, int dst, E e = E()) = 0;

    /**
     * checks if graph has no cycles
     */
    virtual bool is_acyclic() = 0;

    /**
     * checks if there exists euler path in graph
     * puts calculated euler path in result vector
     */
    virtual bool euler_path(VI& result) = 0;

    /**
     * finds all strongly connected components of the graph
     * @return vector with scc
     */
    virtual VVI strongly_connected_components() = 0;

private:

    template<typename EE>
    friend void rich_bfs(const abstract_graph<tree_node, EE> &g, int src);

    template<typename EE>
    friend void rich_dfs(const abstract_graph<timed_node, EE> &g, int src);

    /**
     * finds distances between given node and all other nodes
     * uses Bellman-Ford's algorithm in time O(E*V)
     * works with negative weights
     * @param src starting node
     * @param g graph
     * @return vector with distances
     */
    template<typename V>
    friend VI bellman_ford(int src, const abstract_graph<V, weighted_edge> &g);

    /**
     * finds distances between given node and all other nodes
     * uses Dijkstra's algorithm in time O(E*logV), we probably use heap not fibonacci heap
     * with fibonacci heap the complexity is O(E + V*logV)
     * does not work with negative weights
     * @param src starting node
     * @param g graph
     * @return vector with distances
     */
    template<typename V>
    friend VI dijkstra(int src, const abstract_graph<V, weighted_edge> &g);

    /**
     * finds distances between all pair of nodes
     * uses Floyd-Warshall's algorithm in time O(V^3)
     * @param g graph
     * @return vector with distances
     */
    template<typename V>
    friend VVI floyd_warshall(const abstract_graph<V, weighted_edge> &g);

    /**
     * finds distances between all pair of nodes
     * uses Johnson's algorithm in time O(V*E*logV)
     * with Dijkstra's algorithm with fibonacci heap the complexity is O(V*E + V^2*logV)
     * for sparse graphs it is faster than Floyd-Warshall's algorithm
     * @param g graph
     * @return vector with distances
     */
    template<typename V>
    friend VVI johnson(abstract_graph<V, weighted_edge> &g);

protected:

    /**
     * adds edge e to node src
     * @param src beginning of edge
     * @param e edge to be added
     */
    void _add_edge(int src, edge e);

    /**
     * breadth first search method, used by other algorithms
     * time complexity O(V + E)
     * @param src node to start in
     * @param applier applier applied to node when it is processed
     * @param child_applier applier applied to node when it is added to queue (accept node and its parent)
     * @param predicate function to check if node can be visit (if hasn't been visited already),
     * accepts parent and edge index
     */
    void _bfs(int src,
              const std::function<void(int)> &applier,
              const std::function<void(int, int)> &child_applier,
              const std::function<bool(int, int)> &predicate);

    /**
     * depth first search method, used by other algorithms
     * time complexity O(V + E)
     * @param src node to start in_applier
     * @param visited bitset to mark already visited nodes
     * @param in_applier function applied to node when search reaches the node
     * @param out_applier function applied to node when search exits from node
     * @param predicate function to check if node can be visit (if hasn't been visited already)
     */
    void _dfs_rec(int src, bool *visited,
                  const std::function<void(int)> &in_applier,
                  const std::function<void(int)> &out_applier,
                  const std::function<bool(int)> &predicate);

    /**
     * disconnects node with given index
     * @param index
     */
    virtual void disconnect_node(int index);

};



///IMPLEMENTATION*******************************************************************************************************

template<typename N, typename E>
struct abstract_graph<N, E>::node : N, std::vector<edge> {
    explicit node(N n = N()) : N(n) {}
};

template<typename N, typename E>
struct abstract_graph<N, E>::edge : E {
    int dst;

    explicit edge(int dst, E e = E()) : E(e), dst(dst) {}
};

template<typename N, typename E>
void abstract_graph<N, E>::print() {
    REP(i, size()) {
        std::cout << i << ": ";
        for (edge e: nodes[i]) {
            std::cout << e.dst << " ";
        }
        std::cout << std::endl;
    }
}

template<typename N, typename E>
size_t abstract_graph<N, E>::size() const {
    return nodes.size();
}

template<typename N, typename E>
void abstract_graph<N, E>::_add_edge(int src, abstract_graph::edge e) {
    nodes[src].push_back(e);
}

template<typename N, typename E>
void abstract_graph<N, E>::add_node(N n) {
    nodes.push_back(node(n));
}

template<typename N, typename E>
void abstract_graph<N, E>::disconnect_node(int index) {
    REP(i, size()) {
        nodes[i].erase(std::remove_if(nodes[i].begin(), nodes[i].end(),
                                      [&index](edge const &e) {
                                          return e.dst == index;
                                      }), nodes[i].end());
    }
    nodes[index].clear();
}

template<typename N, typename E>
void abstract_graph<N, E>::_bfs(int src,
                                const std::function<void(int)> &applier,
                                const std::function<void(int, int)> &child_applier,
                                const std::function<bool(int, int)> &predicate) {
    QI q;
    q.push_back(nodes[src]);
    std::bitset<size()> visited;
    while (!q.empty()) {
        int v = q.front();
        q.pop_front();
        applier(v);
        visited[v] = true;
        REP(i, nodes[v].size()) {
            if (!visited[nodes[v][i].dst] && predicate(v, i)) {
                child_applier(nodes[v][i].dst, v);
                q.push_back(nodes[nodes[v][i].dst]);
            }
        }
    }
}

template<typename N, typename E>
void abstract_graph<N, E>::_dfs_rec(int src,
                                    bool *visited,
                                    const std::function<void(int)> &in_applier,
                                    const std::function<void(int)> &out_applier,
                                    const std::function<bool(int)> &predicate) {
    in_applier(src);
    visited[src] = true;
    for (edge e: nodes[src]) {
        if (!visited[e.dst] && predicate(e.dst)) {
            _dfs_rec(e.dst, visited, in_applier, out_applier, predicate);
        }
    }
    out_applier(src);
}

template<typename N>
VI dijkstra(int src, const abstract_graph<N, weighted_edge> &g) {
    VI result(g.size(), 1e9);
    result[src] = 0;
    PQN q;
    REP(i, g.size()) {
        q.push(MP(result[i], i));
    }
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();;
        for (auto e: g.nodes[u]) {
            if (result[u] + e.value < result[e.dst]) {
                q.push(MP(result[u] + e.value, e.dst));
                result[e.dst] = result[u] + e.value;
            }
        }
    }
    return result;
}

template<typename N>
VI bellman_ford(int src, const abstract_graph<N, weighted_edge> &g) {
    VI result(g.size(), 1e9);
    result[src] = 0;
    REP(i, g.size() - 1) {
        REP(j, g.size()) {
            for (auto e: g.nodes[j]) {
                if (result[j] + e.value < result[e.dst]) {
                    result[e.dst] = result[j] + e.value;
                }
            }
        }
    }
    return result;
}

template<typename N>
VVI floyd_warshall(const abstract_graph<N, weighted_edge> &g) {
    VVI result(g.size());
    REP(i, g.size()) {
        REP(j, g.size()) {
            result[i].push_back(1e9);
        }
        result[i][i] = 0;
    }
    REP(i, g.size()) {
        for (auto e: g.nodes[i]) {
            result[i][e.dst] = e.value;
        }
    }
    REP(k, g.size()) {
        REP(i, g.size()) {
            REP(j, g.size()) {
                if (result[i][j] > result[i][k] + result[k][j]) {
                    result[i][j] = result[i][k] + result[k][j];
                }
            }
        }
    }
    return result;
}

template<typename N>
VVI johnson(abstract_graph<N, weighted_edge> &g) {
    VVI result;
    int n = g.size();
    g.add_node();
    REP(i, n) {
        g.add_edge(n, i, weighted_edge());
    }
    VI dist = bellman_ford(n, g);
    REP(i, n) {
        for (auto e: g.nodes[i]) {
            g.nodes[i][e.dst].value += dist[i] - dist[e.dst];
        }
    }
    g.disconnect_node(n);
    g.nodes.erase(--g.nodes.end());
    REP(i, n) {
        VI r = dijkstra(i, g);
        result.push_back(r);
    }
    REP(i, n) {
        for (auto e: g.nodes[i]) {
            g.nodes[i][e.dst].value -= dist[i] - dist[e.dst];
        }
    }
    return result;
}

template<typename EE>
void rich_bfs(const abstract_graph<tree_node, EE> &g, int src) {
    int level = 0, parent = 0;
    g._bfs(src, [&level](int x) { level++; }, [g, &level](int x, int p) {
               g.nodes[x].level = level;
               g.nodes[x].parent = p;
           },
           TRUE_PREDICATE(x));
}

template<typename EE>
void rich_dfs(const abstract_graph<timed_node, EE> &g, int src) {
    int time = 0;
    bool *visited = new bool[g.size()];
    g._dfs_rec(src, visited, [g, &time](int x) { g.nodes[x].time_in = time++; },
               [g, &time](int x) { g.nodes[x].time_out = time++; }, TRUE_PREDICATE(x));
    delete[] visited;
}


#endif //ALGORITHMS_ABSTRACT_GRAPH_H
