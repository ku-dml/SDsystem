#ifndef __TWO_EDGE_CONNECTED_
#define __TWO_EDGE_CONNECTED_
#include "lib.hpp"
#include "graph.hpp"
#include <tuple>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/decomposition/StaticSPQRTree.h>

class TwoEdgeConnected {
    public:
        graph::Graph given_;
        integer num_count_;
    private:
        std::vector<std::tuple<std::vector<integer>, std::bitset<N>>> _candidates_eq_two_;
        std::unordered_map<integer, integer> _v_id_to_can_id_; // for mapping B(P) to CAN>2(C)
        std::vector<std::tuple<integer, std::bitset<N>>> _candidates_gt_two_;
        std::vector<std::tuple<integer, integer>> _edges_incident_to_can_gt_two_; // index x index

    public:
        explicit TwoEdgeConnected(graph::Graph graph) : given_(graph), num_count_(0) {}
        void output(std::bitset<N> &C);
        std::optional<std::bitset<N>> computeMrs(std::bitset<N> &C, std::bitset<N> &X);
        std::vector<std::bitset<N>> computeAllMrs(std::bitset<N> &C, std::bitset<N> &X);
    private:
        void _find_vertices_of_auxiliary_graph(std::bitset<N>& C_v_in_G);
        void _enumerate_candidate(std::bitset<N> &C);
        void _enumerate_edges_itcgt_after_candidates(std::bitset<N> &C);
        std::optional<std::bitset<N>> _computeMrsOfCycle(graph::Graph &G_C_v, std::bitset<N> &C_v, std::bitset<N> &X, std::set<integer> &articulation_points, graph::Graph &G_C);
        std::optional<std::bitset<N>> _computeMrsOfVComponent(graph::Graph &G_C_v, std::bitset<N> &C_v, std::bitset<N> &X, std::set<integer> &articulation_points, graph::Graph& G_C, std::bitset<N>& articulation_points_bit);
        void _computeAllMrsOfVComponent(graph::Graph &G_C_v, std::bitset<N> &C_v, std::bitset<N> &X, std::set<integer> &articulation_points, graph::Graph& G_C, std::bitset<N>& articulation_points_bit, std::vector<std::bitset<N>>& MRSs);
        ogdf::Graph _generate_auxiliary_graph_and_set_candidates(std::bitset<N>& C_v_in_G, graph::Graph& G_C_v, graph::Graph& G_C);
        std::tuple<integer, integer> _compute_boundaries(graph::Graph& G_C_v, integer v);
        void _decide_whether_candidates_is_mrs(ogdf::StaticSPQRTree &SPQRTree, std::vector<bool> &is_mrs);
};

#endif