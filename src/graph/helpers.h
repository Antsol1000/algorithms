//
// Created by Antoni Solarski
// github.com/Antsol1000
//


#ifndef ALGORITHMS_HELPERS_H
#define ALGORITHMS_HELPERS_H

#include <vector>
#include <deque>
#include <queue>
#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>
#include <iostream>
#include <bitset>
#include "structs.h"
#include "../disjoint_sets/disjoint_sets.h"

#define MP(a, b) std::make_pair((a), (b))
#define ALL(a) (a).begin(), (a).end()
#define SORT(a) std::sort(ALL(a))
#define REV(a) std::reverse(ALL(a))
#define REP(i, a) for(int (i) = 0; (i) < (a); (i)++)
#define FOR(i, a) for(auto i = (a).begin(); i != (a).end(); i++)
#define DUMMY_APPLIER(a) [](int a) {}
#define TRUE_PREDICATE(a) [](int a) {return true;}

typedef std::vector<int> VI;
typedef std::vector<std::vector<int>> VVI;
typedef std::vector<std::pair<int, int>> VIN;
typedef std::vector<std::pair<int, std::pair<int, int>>> VIE;
typedef std::deque<int> QI;
typedef std::priority_queue<std::pair<int, int>> PQN;
typedef std::priority_queue<std::pair<int, std::pair<int, int>>> PQE;

#endif //ALGORITHMS_HELPERS_H
