#include "src/disjoint_sets/disjoint_sets.h"
#include "src/big_number/big_number.h"
#include "src/graph/graph.h"
#include <windows.h>

int main() {
    graph<empty, weighted_edge> g(4);
    g.add_edge(0, 1, weighted_edge(3));
    g.add_edge(1, 2, weighted_edge(4));
    g.add_edge(1, 3, weighted_edge(4));
    g.add_edge(2, 3, weighted_edge(10));
    g.print();
    std::cout << "\n\n\n";
    VVI a = floyd_warshall(g);
    REP(i, a.size()) {
        REP(j, g.size()) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    VVI b = johnson(g);
    REP(i, g.size()) {
        REP(j, g.size()) {
            std::cout << b[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n\n\n";
    std::cout << g.to_dot() << std::endl;
    std::string s = "echo " + g.to_dot() + " | dot -Tbmp > output.bmp";
    std::cout << s << "\n";
    system(s.c_str());
    return 0;
}
