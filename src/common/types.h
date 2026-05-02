#pragma once
#include "../graph/graph.h"

namespace DepAnalyser::Types {

    struct Edge {
        const Graph::Vertex* from;
        const Graph::Vertex* to;
    };

}