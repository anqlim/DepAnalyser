cmake_minimum_required(VERSION 3.28)
project(DepAnalyser)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(SOURCES
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

# основное приложение
add_executable(DepAnalyser src/main.cpp ${SOURCES}
        src/server/analysis_service.cpp
        src/server/analysis_service.h)

# Json
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG master
)
FetchContent_MakeAvailable(nlohmann_json)

# Asio
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG master
)
FetchContent_MakeAvailable(asio)

# Crow
FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG master
)
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/include CACHE PATH "" FORCE)
FetchContent_MakeAvailable(crow)

target_include_directories(DepAnalyser PRIVATE
        ${asio_SOURCE_DIR}/include
        ${crow_SOURCE_DIR}/include)

target_compile_definitions(DepAnalyser PRIVATE
        ASIO_STANDALONE
        ASIO_NO_DEPRECATED)

# hiredis
FetchContent_Declare(
        hiredis
        GIT_REPOSITORY https://github.com/redis/hiredis.git
        GIT_TAG        v1.2.0
)

# redis-plus-plus
FetchContent_Declare(
        redis-plus-plus
        GIT_REPOSITORY https://github.com/sewenew/redis-plus-plus.git
        GIT_TAG        1.3.15
)
FetchContent_MakeAvailable(hiredis)

file(MAKE_DIRECTORY ${hiredis_SOURCE_DIR}/hiredis)
file(GLOB HIREDIS_HEADERS "${hiredis_SOURCE_DIR}/*.h")
foreach(HEADER ${HIREDIS_HEADERS})
    get_filename_component(HEADER_NAME ${HEADER} NAME)
    configure_file(${HEADER} ${hiredis_SOURCE_DIR}/hiredis/${HEADER_NAME} COPYONLY)
endforeach()

set(HIREDIS_HEADER ${hiredis_SOURCE_DIR} CACHE PATH "" FORCE)
set(HIREDIS_LIB hiredis::hiredis CACHE STRING "" FORCE)

FetchContent_MakeAvailable(redis-plus-plus)

target_link_libraries(DepAnalyser PRIVATE
        nlohmann_json::nlohmann_json
        hiredis::hiredis
        redis++)

# тесты
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(DepAnalyserTests
        ${SOURCES}
        tests/graph_test.cpp
        tests/cycle_detector_test.cpp
        tests/critical_vertex_test.cpp
        tests/redundancy_test.cpp
        tests/helpers.h
        tests/parser_test.cpp)

target_link_libraries(DepAnalyserTests gtest_main)
add_test(NAME DepAnalyserTests COMMAND DepAnalyserTests)