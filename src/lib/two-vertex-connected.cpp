#include "two-vertex-connected.hpp"
#include "util.hpp"
#include "graph.hpp"
#include <iostream>
#include <algorithm>


using namespace std;
using namespace graph;

void TwoVertexConnected::output(std::bitset<N>& C) {
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

optional<bitset<N>> TwoVertexConnected::computeMrs(bitset<N> &C, bitset<N> &X) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::computeMrs" << endl;
    #endif
    // generate G[C]
    auto G_C = given_.generateInducedGraphFromBitset(C);
    if (G_C.n_ <= 3 || G_C.is_cycle()) {
        return nullopt;
    }

    // decompose G[C] to 2-vertex-connected components
    G_C.enumerate_two_vertex_connected_components();
    G_C.enumerate_articulation_points();
    auto articulation_points = set<integer>();
    auto articulation_points_bit = bitset<N>();
    for (auto& e: G_C.articulation_points_) {
        articulation_points.emplace(G_C.vmap_inv_[e]);
        articulation_points_bit.set(G_C.vmap_inv_[e]);
    }

    optional<bitset<N>> Y = nullopt;
    for (auto &C_v : G_C.two_vertex_connected_components_) {
        auto G_C_v = G_C.generateInducedGraphFromBitset(C_v);
        if (G_C_v.is_cycle()) {
            // unnecessary
            Y = _computeMrsOfCycle(G_C_v, C_v, X, articulation_points, G_C);
        } else {
            Y = _computeMrsOfVComponent(G_C_v, C_v, X, articulation_points, G_C, articulation_points_bit);
        }
        if (Y.has_value()) {
            return Y;
        }
    }
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::computeMrs<" << endl;
    #endif
    return nullopt;
}

optional<bitset<N>> TwoVertexConnected::_computeMrsOfVComponent(Graph& G_C_v, bitset<N>& C_v, bitset<N>& X, set<integer>& articulation_points, Graph& G_C, bitset<N>& articulation_points_bit) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_computeMrsOfVComponent" << endl;
    #endif
    auto C_v_in_G = G_C.inverse_bitset(C_v);

    // 1. find CAN>2(C), CAN=2(C), and Lambda>2(C)
    // 2. generate auxiliary graph
    auto auxiliary_graph = _generate_auxiliary_graph_and_set_candidates(C_v_in_G, G_C_v, G_C);
    // 3. generate SPQR-tree of auxiliary graph
    auto SPQR_tree = ogdf::StaticSPQRTree(auxiliary_graph);
    // 4. test whether Y in CAN(C) is MRS or not
    auto a = _candidates_gt_two_.size();
    auto b = _candidates_eq_two_.size();
    auto is_mrs = vector<bool>(a + b, true);
    // for all P in CAN=2(C), B(P) is not a MRS
    // for (integer i = 0; i < _candidates_eq_two_.size(); ++i) {
    //     auto [vertics, TwoDegPath] = _candidates_eq_two_[i];
    //     auto [u_in_C_v, v_in_C_v] = _compute_boundaries(G_C_v, G_C_v.vmap_[G_C.vmap_[vertics[0]]]);
    //     auto u_in_G_C = G_C_v.vmap_inv_[u_in_C_v];
    //     auto v_in_G_C = G_C_v.vmap_inv_[v_in_C_v];
    //     auto u_in_G = G_C.vmap_inv_[u_in_G_C];
    //     auto v_in_G = G_C.vmap_inv_[v_in_G_C];
    //     auto can_id_u = _v_id_to_can_id_[u_in_G];
    //     auto can_id_v = _v_id_to_can_id_[v_in_G];
    //     is_mrs[can_id_u] = false;
    //     is_mrs[can_id_v] = false;
    // }
    _decide_whether_candidates_is_mrs(SPQR_tree, is_mrs);

    optional<bitset<N>> MRS = nullopt;
    for (integer i = 0; i < a + b; ++i) {
        if (is_mrs[i]) {
            if (i < a) {
                auto [_, Y] = _candidates_gt_two_[i];
                MRS = Y;
            } else {
                auto [_, Y] = _candidates_eq_two_[i - a];
                MRS = Y;
            }
            if ((MRS.value() & X).any() || (MRS.value() & articulation_points_bit).any()) {
                MRS = nullopt;
                continue;
            }
            break;
        }
    }
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_computeMrsOfVComponent<" << endl;
    #endif
    return MRS;
}

void TwoVertexConnected::_computeAllMrsOfVComponent(graph::Graph &G_C_v, std::bitset<N> &C_v, std::bitset<N> &X, std::set<integer> &articulation_points, graph::Graph& G_C, std::bitset<N>& articulation_points_bit, std::vector<std::bitset<N>>& MRSs) {
    auto C_v_in_G = G_C.inverse_bitset(C_v);

    // 1. find CAN>2(C), CAN=2(C), and Lambda>2(C)
    // 2. generate auxiliary graph
    auto auxiliary_graph = _generate_auxiliary_graph_and_set_candidates(C_v_in_G, G_C_v, G_C);
    // 3. generate SPQR-tree of auxiliary graph
    auto SPQR_tree = ogdf::StaticSPQRTree(auxiliary_graph);
    // 4. test whether Y in CAN(C) is MRS or not
    auto a = _candidates_gt_two_.size();
    auto b = _candidates_eq_two_.size();
    auto is_mrs = vector<bool>(a + b, true);
    // for all P in CAN=2(C), B(P) is not a MRS
    // for (integer i = 0; i < _candidates_eq_two_.size(); ++i) {
    //     auto [vertics, TwoDegPath] = _candidates_eq_two_[i];
    //     auto [u_in_C_v, v_in_C_v] = _compute_boundaries(G_C_v, G_C_v.vmap_[G_C.vmap_[vertics[0]]]);
    //     auto u_in_G_C = G_C_v.vmap_inv_[u_in_C_v];
    //     auto v_in_G_C = G_C_v.vmap_inv_[v_in_C_v];
    //     auto u_in_G = G_C.vmap_inv_[u_in_G_C];
    //     auto v_in_G = G_C.vmap_inv_[v_in_G_C];
    //     auto can_id_u = _v_id_to_can_id_[u_in_G];
    //     auto can_id_v = _v_id_to_can_id_[v_in_G];
    //     is_mrs[can_id_u] = false;
    //     is_mrs[can_id_v] = false;
    // }
    _decide_whether_candidates_is_mrs(SPQR_tree, is_mrs);

    optional<bitset<N>> MRS = nullopt;
    for (integer i = 0; i < a + b; ++i) {
        if (is_mrs[i]) {
            if (i < a) {
                auto [_, Y] = _candidates_gt_two_[i];
                MRS = Y;
            } else {
                auto [_, Y] = _candidates_eq_two_[i - a];
                MRS = Y;
            }
            if ((MRS.value() & X).any() || (MRS.value() & articulation_points_bit).any()) {
                MRS = nullopt;
            }
            if (MRS.has_value()) {
                MRSs.push_back(MRS.value());
                MRS = nullopt;
            }
        }
    }
}

void TwoVertexConnected::_find_vertices_of_auxiliary_graph(std::bitset<N>& C_v_in_G) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_find_vertices_of_auxiliary_graph" << endl;
    #endif

    // find CAN>2(C) and CAN=2(C), and make map (v_id to can_id)
    _enumerate_candidate(C_v_in_G);
    // find Lambda>2(C) and contain can_id x can_id
    _enumerate_edges_itcgt_after_candidates(C_v_in_G);

    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_find_vertices_of_auxiliary_graph<" << endl;
    #endif
}

ogdf::Graph TwoVertexConnected::_generate_auxiliary_graph_and_set_candidates(std::bitset<N>& C_v_in_G, Graph& G_C_v, Graph& G_C) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_generateAuxiliaryGraphAndSetCandidates" << endl;
    #endif
    _find_vertices_of_auxiliary_graph(C_v_in_G);

    auto a = _candidates_gt_two_.size();
    auto b = _candidates_eq_two_.size();
    // auto c = _edges_incident_to_can_gt_two_.size();

    ogdf::Graph G;
    auto node_container = vector<ogdf::node>();
    // for (integer i = 0; i < a + b + c; ++i) {
    for (integer i = 0; i < a + b; ++i) {
        // CAN>2(C_v) = [0, a-1]
        // CAN=2(C_v) = [a, a+b-1]
        // Λ>2(C_v) = [a+b, a+b+c-1]
        node_container.push_back(G.newNode()); // keep address (pointer)
    }
    // {uP \in CAN>2(C) x CAN=2(C) |  u \in B(P)}
    for (integer i = 0; i < _candidates_eq_two_.size(); ++i) {
        auto path_id = a + i;
        auto [vertics, TwoDegPath] = _candidates_eq_two_[i];
        auto [u_in_C_v, v_in_C_v] = _compute_boundaries(G_C_v, G_C_v.vmap_[G_C.vmap_[vertics[0]]]);
        auto u_in_G_C = G_C_v.vmap_inv_[u_in_C_v];
        auto v_in_G_C = G_C_v.vmap_inv_[v_in_C_v];
        auto u_in_G = G_C.vmap_inv_[u_in_G_C];
        auto v_in_G = G_C.vmap_inv_[v_in_G_C];
        auto can_id_u = _v_id_to_can_id_[u_in_G];
        auto can_id_v = _v_id_to_can_id_[v_in_G];
        G.newEdge(node_container[path_id], node_container[can_id_u]);
        G.newEdge(node_container[path_id], node_container[can_id_v]);
    }
    // {ue \in CAN>2(C) x Λ>2(C) | u \in e}
    for (integer i = 0; i < _edges_incident_to_can_gt_two_.size(); ++i) {
        // auto edge_id = a + b + i;
        auto [u_id, v_id] = _edges_incident_to_can_gt_two_[i];
        // G.newEdge(node_container[edge_id], node_container[u_id]);
        // G.newEdge(node_container[edge_id], node_container[v_id]);
        G.newEdge(node_container[u_id], node_container[v_id]);
    }
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_generateAuxiliaryGraphAndSetCandidates<" << endl;
    #endif
    return G;
}


void TwoVertexConnected::_decide_whether_candidates_is_mrs(ogdf::StaticSPQRTree &SPQRTree, std::vector<bool> &is_mrs) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_decide_whether_candidates_is_mrs" << endl;
    #endif

    auto a = _candidates_gt_two_.size();
    auto b = _candidates_eq_two_.size();
    // 1. endpoints of virtual edge is cut point pair
    for (auto &v : SPQRTree.nodesOfType(ogdf::SPQRTree::NodeType::SNode)) {
        auto g = SPQRTree.skeleton(v).getGraph();
        for (auto& e: g.edges) {
            if (SPQRTree.skeleton(v).isVirtual(e)) {
                auto [x, y] = e->nodes();
                auto l = SPQRTree.skeleton(v).original(x)->index();
                auto r = SPQRTree.skeleton(v).original(y)->index();
                if (l < a && r < a + b) {
                    // l is CAN>2(C),
                    // r is CAN(C)
                    // {l, r} is cut point pair
                    is_mrs[r] = false;
                }
                if (l < a + b && r < a) {
                    // l is CAN(C),
                    // r is CAN>2(C)
                    // {l, r} is cut point pair
                    is_mrs[l] = false;
                }
            }
        }
    }
    for (auto &v : SPQRTree.nodesOfType(ogdf::SPQRTree::NodeType::PNode)) {
        auto g = SPQRTree.skeleton(v).getGraph();
        for (auto& e: g.edges) {
            if (SPQRTree.skeleton(v).isVirtual(e)) {
                auto [x, y] = e->nodes();
                auto l = SPQRTree.skeleton(v).original(x)->index();
                auto r = SPQRTree.skeleton(v).original(y)->index();
                if (l < a && r < a + b) {
                    // l is CAN>2(C),
                    // r is CAN(C)
                    // {l, r} is cut point pair
                    is_mrs[r] = false;
                }
                if (l < a + b && r < a) {
                    // l is CAN(C),
                    // r is CAN>2(C)
                    // {l, r} is cut point pair
                    is_mrs[l] = false;
                }
            }
        }
    }
    for (auto& v: SPQRTree.nodesOfType(ogdf::SPQRTree::NodeType::RNode)) {
        auto g = SPQRTree.skeleton(v).getGraph();
        for (auto& e: g.edges) {
            if (SPQRTree.skeleton(v).isVirtual(e)) {
                auto [x, y] = e->nodes();
                auto l = SPQRTree.skeleton(v).original(x)->index();
                auto r = SPQRTree.skeleton(v).original(y)->index();
                if (l < a && r < a + b) {
                    // l is CAN>2(C),
                    // r is CAN(C)
                    // {l, r} is cut point pair
                    is_mrs[r] = false;
                }
                if (l < a + b && r < a) {
                    // l is CAN(C),
                    // r is CAN>2(C)
                    // {l, r} is cut point pair
                    is_mrs[l] = false;
                }
            }
        }
    }
    // 2. pair of vertices in S node that are not adjacent to each other is cut point pair
    for (auto &v : SPQRTree.nodesOfType(ogdf::SPQRTree::NodeType::SNode)) {
        auto g = SPQRTree.skeleton(v).getGraph();
        auto number_of_ordinary_vertex = 0;
        auto number_of_adjacent_ordinary_vertex = vector<integer>(g.numberOfNodes(), 0);
        auto original_id = vector<integer>(g.numberOfNodes(), 0);
        for (auto &e : g.edges) {
            auto [x, y] = e->nodes();
            auto x_original = SPQRTree.skeleton(v).original(x)->index();
            auto y_original = SPQRTree.skeleton(v).original(y)->index();
            original_id[x->index()] = x_original;
            original_id[y->index()] = y_original;
            if (x_original < a) {
                // x is ordinary vertex
                ++number_of_ordinary_vertex;
                ++number_of_adjacent_ordinary_vertex[y->index()];
            }
            if (y_original < a) {
                // y is ordinary vertex
                ++number_of_ordinary_vertex;
                ++number_of_adjacent_ordinary_vertex[x->index()];
            }
        }
        number_of_ordinary_vertex /= 2;
        for (integer i = 0; i < g.numberOfNodes(); ++i) {
            if (number_of_adjacent_ordinary_vertex[i] != number_of_ordinary_vertex) {
                if (original_id[i] >= a + b) {
                    continue; // not candidate
                }
                if (original_id[i] < a) { // if i is CAN>2(C)
                    if (number_of_adjacent_ordinary_vertex[i] + 1 == number_of_ordinary_vertex) {
                        // i itself is auxiliary vertex
                        continue;
                    }
                }
                is_mrs[original_id[i]] = false;
            }
        }
    }

    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_decide_whether_candidates_is_mrs<" << endl;
    #endif
}

tuple<integer, integer> TwoVertexConnected::_compute_boundaries(graph::Graph& G_C_v, integer v) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_compute_boundaries" << endl;
        cout << "\tinput v: " << v << endl;
        cout << "\tinput n: " << G_C_v.n_ << endl;
        given_.output_relations_between_name_and_id();

        for (auto& v: G_C_v.V_) {
            cout << "id: " << G_C_v.vmap_inv_[v.id_] << ", deg: " << v.deg_ << endl;
            cout << "neighbors: ";
            for (auto& v: v.neighbors_) {
                cout << G_C_v.vmap_inv_[v] << ",";
            }
            cout << endl;
        }
    #endif

    auto left = G_C_v.V_[v].neighbors_[0];
    auto left_prev = v;
    while (G_C_v.V_[left].deg_ <= 2) {
        // degree of boundary > 2
        if (G_C_v.V_[left].neighbors_[0] != left_prev) {
            left_prev = left;
            left = G_C_v.V_[left].neighbors_[0];
        } else {
            left_prev = left;
            left = G_C_v.V_[left].neighbors_[1];
        }
    }
    auto right = G_C_v.V_[v].neighbors_[1];
    auto right_prev = v;
    while (G_C_v.V_[right].deg_ <= 2) {
        // degree of boundary > 2
        if (G_C_v.V_[right].neighbors_[0] != right_prev) {
            right_prev = right;
            right = G_C_v.V_[right].neighbors_[0];
        } else {
            right_prev = right;
            right = G_C_v.V_[right].neighbors_[1];
        }
    }

    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_compute_boundaries<" << endl;
    #endif
    return {left, right};
}

optional<bitset<N>> TwoVertexConnected::_computeMrsOfCycle(Graph& G_C_v, bitset<N>& C_v, bitset<N>& X, set<integer>& articulation_points, Graph& G_C) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_computeMrsOfCycle" << endl;
    #endif
    optional<integer> art = nullopt;
    bitset<N> Y;
    for (auto &v: G_C_v.V_) {
        auto v_id_in_G_C_v = v.id_;
        auto v_id_in_G_C = G_C_v.vmap_inv_[v_id_in_G_C_v];
        auto v_id_in_G = G_C.vmap_inv_[v_id_in_G_C];
        if (articulation_points.find(v_id_in_G) != articulation_points.end()) {
            if (art.has_value()) {
                return nullopt; // C_v contains more than one articulation point
            }
            art = v_id_in_G;
        } else {
            Y.set(v_id_in_G);
        }
    }
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_computeMrsOfCycle<" << endl;
    #endif
    if ((Y & X).any()) {
        return nullopt;
    } else {
        return Y;
    }
}

void TwoVertexConnected::_enumerate_candidate(bitset<N>& C) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_enumerate_candidate" << endl;
    #endif
    _candidates_eq_two_.clear();
    _v_id_to_can_id_.clear();
    _candidates_gt_two_.clear();
    // generate G[C]
    auto G_C = given_.generateInducedGraphFromBitset(C);
    if (G_C.is_cycle()) {
        return;
    }
    // find v s.t. deg(v) > 2
    auto CAN_G_2 = bitset<N>();
    for (integer u = 0; u < G_C.n_; ++u) {
        if (G_C.V_[u].deg_ > 2) {
            // {u} is candidate
            auto u_in_G = G_C.vmap_inv_[u];
            auto Y_left = u_in_G;
            auto Y_bit = bitset<N>();
            Y_bit.set(u_in_G);
            _v_id_to_can_id_.emplace(u_in_G, _candidates_gt_two_.size());
            _candidates_gt_two_.emplace_back(Y_left, Y_bit);

            // remove CAN_>2(C)
            C.reset(u_in_G);
            CAN_G_2.set(u_in_G); // in order to restore C
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
        _candidates_eq_two_.emplace_back(Y_vec, Y_bit);
    }

    // restore C
    C |= CAN_G_2;
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_enumerate_candidate<" << endl;
    #endif
}

void TwoVertexConnected::_enumerate_edges_itcgt_after_candidates(std::bitset<N> &C) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_enumerate_edges_itcgt_after_candidates" << endl;
    #endif
    _edges_incident_to_can_gt_two_.clear();
    // O(E)
    for (auto v: given_.V_) {
        if (!C[v.id_]) {
            // not in C
            continue;
        }
        if (_v_id_to_can_id_.find(v.id_) == _v_id_to_can_id_.end()) {
            // not in CAN>2(C)
            continue;
        }
        for (auto u: v.neighbors_) {
            if (v.id_ > u) {
                continue; // in our implementation, there are (u, v) and (v, u)
            }
            if (_v_id_to_can_id_.find(u) != _v_id_to_can_id_.end()) {
                // u is also in CAN>2(C)
                _edges_incident_to_can_gt_two_.emplace_back(minmax(_v_id_to_can_id_[v.id_], _v_id_to_can_id_[u]));
            }
        }
    }
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::_enumerate_edges_itcgt_after_candidates<" << endl;
    #endif
}

std::vector<std::bitset<N>> TwoVertexConnected::computeAllMrs(std::bitset<N> &C, std::bitset<N> &X) {
    #ifdef __DEBUG_
        cout << "TRACE: TwoVertexConnected::computAllMrs" << endl;
    #endif
    vector<bitset<N>> MRSs = {};
    // generate G[C]
    auto G_C = given_.generateInducedGraphFromBitset(C);
    if (G_C.n_ <= 3 || G_C.is_cycle()) {
        return MRSs;
    }

    // decompose G[C] to 2-vertex-connected components
    G_C.enumerate_two_vertex_connected_components();
    G_C.enumerate_articulation_points();
    auto articulation_points = set<integer>();
    auto articulation_points_bit = bitset<N>();
    for (auto& e: G_C.articulation_points_) {
        articulation_points.emplace(G_C.vmap_inv_[e]);
        articulation_points_bit.set(G_C.vmap_inv_[e]);
    }

    for (auto &C_v : G_C.two_vertex_connected_components_) {
        auto G_C_v = G_C.generateInducedGraphFromBitset(C_v);
        if (G_C_v.n_ <= 1) {
            exit(1);
        }
        if (G_C_v.is_cycle()) {
            auto Y = _computeMrsOfCycle(G_C_v, C_v, X, articulation_points, G_C);
            if (Y.has_value()) {
                MRSs.push_back(Y.value());
            }
        } else {
            _computeAllMrsOfVComponent(G_C_v, C_v, X, articulation_points, G_C, articulation_points_bit, MRSs);
        }
    }
    return MRSs;
}