#include "two-edge-connected-square.hpp"
#include "util.hpp"
#include "graph.hpp"
#include <iostream>

using namespace std;
using namespace graph;

void TwoEdgeConnectedSquare::output(std::bitset<N>& C) {
    num_count_++;
    // cout << "{";
    // integer i = 0;
    // for (; i < given_.n_; ++i) { // find first element
    //     if (C[i]) {
    //         cout << i;
    //         ++i;
    //         break;
    //     }
    // }
    // for (; i < given_.n_; ++i) { // find the rest
    //     if (C[i]) {
    //         cout << "," << i;
    //     }
    // }
    // cout << "}" << endl;
}

optional<bitset<N>> TwoEdgeConnectedSquare::computeMrs(bitset<N> &C, bitset<N> &X) {
    // enumerate Can(C)
    _enumerate_candidate(C);
    // decide wheter Y \in Can(C) is MRS
    for (auto& candidate: _candidates_) {
        auto [_, Y_bit] = candidate;
        if ((X & Y_bit).any()) { // X \cap Y is not \emptyset
            continue;
        }
        setMinus(C, Y_bit);
        auto G_C_minus_Y = given_.generateInducedGraphFromBitset(C);
        C |= Y_bit; // (C \setminus Y) \cup Y

        G_C_minus_Y.enumearte_bridges();
        if (G_C_minus_Y.bridges_.empty() && G_C_minus_Y.is_connected()) {
            return Y_bit;
        }
    }
    // return
    return nullopt;
}

void TwoEdgeConnectedSquare::_enumerate_candidate(bitset<N>& C) {
    _candidates_.clear();
    // generate G[C]
    auto G_C = given_.generateInducedGraphFromBitset(C);
    if (G_C.is_cycle() || G_C.n_ == 1) {
        return;
    }
    // find v s.t. deg(v) = 1
    auto CAN_G_2 = bitset<N>();
    for (integer u = 0; u < G_C.n_; ++u) {
        if (G_C.V_[u].deg_ > 2) {
            // {u} is candidate
            auto u_in_G = G_C.vmap_inv_[u];
            auto Y_vec = { u_in_G };
            auto Y_bit = bitset<N>();
            Y_bit.set(u_in_G);
            _candidates_.emplace_back(Y_vec, Y_bit);

            // remove CAN_>2(C)
            C.reset(u_in_G);
            CAN_G_2.set(u_in_G);
        }
    }
    // enumerate connected component as CAN_=2(C)
    auto G_C_minus_CAN_G_2 = given_.generateInducedGraphFromBitset(C);
    G_C_minus_CAN_G_2.enumerate_connected_components();
    for (auto &connected_component : G_C_minus_CAN_G_2.connected_components_) {
        // two-deg path
        vector<integer> Y_vec = {};
        auto Y_bit = bitset<N>();
        for (auto &v: connected_component) {
            auto v_in_G = G_C_minus_CAN_G_2.vmap_inv_[v];
            Y_vec.push_back(v_in_G);
            Y_bit.set(v_in_G);
        }
        _candidates_.emplace_back(Y_vec, Y_bit);
    }

    // restore C
    C |= CAN_G_2;
}


std::vector<std::bitset<N>> TwoEdgeConnectedSquare::computeAllMrs(std::bitset<N> &C, std::bitset<N> &X) {
    // enumerate Can(C)
    _enumerate_candidate(C);
    // decide wheter Y \in Can(C) is MRS
    vector<bitset<N>> MRSs = {};
    for (auto& candidate: _candidates_) {
        auto [_, Y_bit] = candidate;
        if ((X & Y_bit).any()) { // X \cap Y is not \emptyset
            continue;
        }
        setMinus(C, Y_bit);
        auto G_C_minus_Y = given_.generateInducedGraphFromBitset(C);
        C |= Y_bit; // (C \setminus Y) \cup Y

        G_C_minus_Y.enumearte_bridges();
        if (G_C_minus_Y.bridges_.empty()  && G_C_minus_Y.is_connected()) {
            MRSs.push_back(Y_bit);
        }
    }
    return MRSs;
}