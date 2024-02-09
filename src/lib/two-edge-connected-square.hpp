#ifndef __TWO_EDGE_CONNECTED_SQUARE_
#define __TWO_EDGE_CONNECTED_SQUARE_
#include "lib.hpp"
#include "graph.hpp"
#include <tuple>

class TwoEdgeConnectedSquare {
    public:
        graph::Graph given_;
        integer num_count_;
    private:
        std::vector<std::tuple<std::vector<integer>, std::bitset<N>>> _candidates_;

    public:
        explicit TwoEdgeConnectedSquare(graph::Graph graph) : given_(graph), num_count_(0) {}
        void output(std::bitset<N> &C);
        std::optional<std::bitset<N>> computeMrs(std::bitset<N> &C, std::bitset<N> &X);
        std::vector<std::bitset<N>> computeAllMrs(std::bitset<N> &C, std::bitset<N> &X);
    private:
        void _enumerate_candidate(std::bitset<N>& C);

};

#endif