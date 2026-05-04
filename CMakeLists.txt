cmake_minimum_required(VERSION 3.30)
project(DepAnalyser)

set(CMAKE_CXX_STANDARD 20)

add_executable(DepAnalyser src/main.cpp
        src/graph/graph.cpp
        src/graph/graph.h
        src/detectors/detector.h
        src/detectors/cycle/cycle.cpp
        src/detectors/cycle/cycle.h
        src/detectors/critical/critical_vertex.cpp
        src/detectors/critical/critical_vertex.h
        src/detectors/redundancy/redundancy.cpp
        src/detectors/redundancy/redundancy.h
        src/common/types.h
        src/parser/parser.h
        src/parser/cpp/cpp_parser.cpp
        src/parser/cpp/cpp_parser.h
        src/parser/python/python_parser.cpp
        src/parser/python/python_parser.h
        src/parser/java/java_parser.cpp
        src/parser/java/java_parser.h
        src/parser/js/js_parser.cpp
        src/parser/js/js_parser.h
        src/parser/go/go_parser.cpp
        src/parser/go/go_parser.h
        src/parser/factory.cpp
        src/parser/factory.h
        src/parser/graph_builder.cpp
        src/parser/graph_builder.h
        src/common/tread_pool/thread_pool.cpp
        src/common/tread_pool/thread_pool.h)
