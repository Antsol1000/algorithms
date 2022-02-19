#include "src/disjoint_sets/disjoint_sets.h"
#include "src/big_number/big_number.h"
#include "src/graph/graph.h"
#include "src/heap/binary_heap.h"
#include "src/heap/fibonacci_heap.h"
#include <queue>

using namespace std;
int main() {

    binary_heap<int> u({2, 3, 1, 4, 0, -1});
    u.remove(4);
    u.remove(-1);
    cout << u.contains(-1) << "\n";
    u.decrease(3, -1);
    cout << u.contains(-1) << "\n";
    while (!u.empty()) {
        cout << u.top() << " ";
        u.pop();
    }

    cout << "\n\n";

    fibonacci_heap<int> v;
    for (auto i : {2, 3, 1, 4, 0, -1}) {
        v.push(i);
    }
    v.remove(4);
    v.remove(-1);
    cout << v.contains(-1) << "\n";
    v.decrease(3, -1);
    cout << v.contains(-1) << "\n";
    while (!v.empty()) {
        cout << v.top() << " ";
        v.pop();
    }

    cout << "\n\n";

    fibonacci_heap<int> a;
    binary_heap<int> b;
    std::priority_queue<int, std::vector<int>, std::greater<>> c;
    for (int i = 0; i < 100; i++) {
        int x = rand();
        a.push(x);
        b.push(x);
        c.push(x);
    }
    for (int i = 0; i < 10; i++) {
        int x = a.top();
        a.pop();
        int y = b.top();
        b.pop();
        int z = c.top();
        c.pop();
        cout << x << " " << y << " " << z << endl;
    }
    for (int i = 0; i < 100; i++) {
        int x = rand();
        a.push(x);
        b.push(x);
        c.push(x);
    }
    for (int i = 0; i < 10; i++) {
        int x = a.top();
        a.pop();
        int y = b.top();
        b.pop();
        int z = c.top();
        c.pop();
        cout << x << " " << y << " " << z << endl;
    }

    return 0;
}
