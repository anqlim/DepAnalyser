#include <gtest/gtest.h>
#include "../src/graph/graph.h"
using namespace DepAnalyser;

TEST(GraphTest, AddVertex) {
    Graph::Graph g;
    auto* v = g.addVertex("main.cpp");
    ASSERT_NE(v, nullptr);
    EXPECT_EQ(v->file_name, "main.cpp");
}

TEST(GraphTest, FindVertex) {
    Graph::Graph g;
    g.addVertex("main.cpp");
    EXPECT_NE(g.findVertex("main.cpp"), nullptr);
    EXPECT_EQ(g.findVertex("unknown.cpp"), nullptr);
}

TEST(GraphTest, AddEdge) {
    Graph::Graph g;
    auto* a = g.addVertex("a.cpp");
    auto* b = g.addVertex("b.cpp");
    g.addEdge(a, b);
    EXPECT_TRUE(a->dependencies.contains(b));
    EXPECT_TRUE(b->dependents.contains(a));
}

TEST(GraphTest, NoDuplicateEdges) {
    Graph::Graph g;
    auto* a = g.addVertex("a.cpp");
    auto* b = g.addVertex("b.cpp");
    g.addEdge(a, b);
    g.addEdge(a, b);
    EXPECT_EQ(a->dependencies.size(), 1);
}