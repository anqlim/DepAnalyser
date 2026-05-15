#include <gtest/gtest.h>
#include "../src/graph/graph.h"
#include "../src/detectors/cycle/cycle.h"
#include "helpers.h"
using namespace DepAnalyser;

TEST(CycleDetectorTest, NoCycles) {
    auto g = buildGraph({{"a", "b"}, {"b", "c"}});
    Detection::CycleDetector d(g);
    d.run();
    EXPECT_TRUE(d.result().empty());
}

TEST(CycleDetectorTest, SimpleCycle) {
    auto g = buildGraph({{"a", "b"}, {"b", "a"}});
    Detection::CycleDetector d(g);
    d.run();
    EXPECT_EQ(d.result().size(), 1);
}

TEST(CycleDetectorTest, EmptyGraph) {
    Graph::Graph g;
    Detection::CycleDetector d(g);
    d.run();
    EXPECT_TRUE(d.result().empty());
}

TEST(CycleDetectorTest, ResultBeforeRun) {
    Graph::Graph g;
    Detection::CycleDetector d(g);
    EXPECT_THROW(d.result(), std::logic_error);
}

TEST(CycleDetectorTest, LongCycle) {
    auto g = buildGraph({{"a", "b"}, {"b", "c"}, {"c", "a"}});
    Detection::CycleDetector d(g);
    d.run();
    EXPECT_EQ(d.result().size(), 1);
    EXPECT_EQ(d.result()[0].size(), 3);
}