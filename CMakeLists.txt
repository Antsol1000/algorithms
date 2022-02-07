cmake_minimum_required(VERSION 3.20)
project(algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(algorithms main.cpp src/disjoint_sets/disjoint_sets.h src/big_number/big_number.cpp src/big_number/big_number.h src/graph/abstract_graph.h src/graph/directed_graph.h src/graph/graph.h src/graph/undirected_graph.h src/graph/helpers.h src/graph/structs.h)
