cmake_minimum_required(VERSION 3.30)
project(DepAnalyser)

set(CMAKE_CXX_STANDARD 20)

add_executable(DepAnalyser src/main.cpp
        src/graph/graph.cpp
        src/graph/graph.h
        src/detectors/detector.h
        src/detectors/cycle.cpp
        src/detectors/cycle.h
        src/detectors/critical_vertex.cpp
        src/detectors/critical_vertex.h
        src/detectors/redundancy.cpp
        src/detectors/redundancy.h
        src/common/types.h)
