#include <gtest/gtest.h>
#include "../src/detectors/critical/critical_vertex.h"
#include "helpers.h"
using namespace DepAnalyser;

TEST(CriticalVertexTest, EmptyGraph) {
    Graph::Graph g;
    Detection::CriticalVertexDetector d(g);
    d.run();
    EXPECT_TRUE(d.result().empty());
}

TEST(CriticalVertexTest, ResultBeforeRun) {
    Graph::Graph g;
    Detection::CriticalVertexDetector d(g);
    EXPECT_THROW(d.result(), std::logic_error);
}

TEST(CriticalVertexTest, SingleVertex) {
    Graph::Graph g;
    g.addVertex("a");
    Detection::CriticalVertexDetector d(g);
    d.run();
    EXPECT_EQ(d.result().size(), 1);
}

TEST(CriticalVertexTest, LinearGraph) {
    // a → b → c → d
    // b и c должны быть критичнее a и d
    auto g = buildGraph({{"a", "b"}, {"b", "c"}, {"c", "d"}});
    Detection::CriticalVertexDetector d(g);
    d.run();
    // одна компонента — один критичный файл
    EXPECT_EQ(d.result().size(), 1);
}

TEST(CriticalVertexTest, TwoComponents) {
    // компонента 1: a → b → c
    // компонента 2: x → y
    auto g = buildGraph({{"a", "b"}, {"b", "c"}, {"x", "y"}});
    Detection::CriticalVertexDetector d(g);
    d.run();
    // две компоненты — два критичных файла
    EXPECT_EQ(d.result().size(), 2);
}

TEST(CriticalVertexTest, CriticalIsMiddle) {
    // a → b → c (только через b)
    auto g = buildGraph({{"a", "b"}, {"b", "c"}});
    Detection::CriticalVertexDetector d(g);
    d.run();
    auto* b = g.findVertex("b");
    EXPECT_TRUE(d.result().contains(b));
}