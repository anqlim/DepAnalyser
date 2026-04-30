cmake_minimum_required(VERSION 3.30)
project(DepAnalyser)

set(CMAKE_CXX_STANDARD 20)

add_executable(DepAnalyser src/main.cpp
        src/graph/graph.cpp)
