#include <gtest/gtest.h>
#include "../src/graph/graph.h"
#include "../src/detectors/cycle/cycle.h"
#include "../src/detectors/redundancy/redundancy.h"
#include "helpers.h"
using namespace DepAnalyser;

TEST(RedundancyTest, NoRedundancy) {
    auto g = buildGraph({{"a", "b"}, {"b", "c"}});
    Detection::CycleDetector cd(g);
    cd.run();
    Detection::RedundancyDetector rd(g, cd.result());
    rd.run();
    EXPECT_TRUE(rd.result().empty());
}

TEST(RedundancyTest, SimpleRedundancy) {
    // a→b, b→c, a→c — последнее избыточно
    auto g = buildGraph({{"a", "b"}, {"b", "c"}, {"a", "c"}});
    Detection::CycleDetector cd(g);
    cd.run();
    Detection::RedundancyDetector rd(g, cd.result());
    rd.run();
    EXPECT_EQ(rd.result().size(), 1);
}