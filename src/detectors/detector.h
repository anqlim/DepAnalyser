#pragma once
#include "../graph/graph.h"
#include <stdexcept>

namespace DepAnalyser::Detection {

    class Detector {
    public:
        explicit Detector(const Graph::Graph& graph) : graph_(graph) {}
        virtual ~Detector() = default;
        virtual void run() = 0;

    protected:
        const Graph::Graph& graph_;
        void markRan() { is_ran_ = true; }
        void checkRan() const {
            if (!is_ran_) throw std::logic_error("run() must be called before result()");
        }

    private:
        bool is_ran_ = false;
    };

}
