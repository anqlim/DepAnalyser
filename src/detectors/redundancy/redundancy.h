#pragma once
#include "../detector.h"
#include "../../common/types.h"
#include <queue>

namespace DepAnalyser::Detection {

        class RedundancyDetector : public Detector {
        public:
            explicit RedundancyDetector(const Graph::Graph& graph,
                                        const std::vector<std::unordered_set<const Graph::Vertex*>>& scc);
            void run() override;
            [[nodiscard]] const std::vector<Types::Edge>& result() const;

        private:
            std::vector<std::unordered_set<const Graph::Vertex*>> scc_;
            std::unordered_map<const Graph::Vertex*, int> scc_index_;
            std::vector<Types::Edge> redundancy_edges_;
            bool bfs(const Graph::Vertex* start, const Graph::Vertex* target);
            void mappingSCC();
        };

}
