#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

#include "graph.hpp"

using namespace std;
using namespace graph;

void assignIdIfNotExists(std::unordered_map<integer, integer> &vmap,
    std::unordered_map<integer, integer> &vmap_inv, std::vector<integer> &vname,
    integer &id, integer vertex_name, std::vector<graph::Vertex> &V);

bool Graph::is_cycle() {
    // simple cycle iff
    // 1. all deg(v) = 2 and
    // 2. connected
    for (integer i = 0; i < n_; ++i) {
        if (V_[i].deg_ != 2) {
            return false;
        }
    }
    return is_connected();
}

bool Graph::is_connected() {
    if (n_ == 0) {
        return true;
    }
    for (integer i = 0; i < n_; ++i) {
        _visited_[i] = false;
    }
    _visited_[0] = true;
    _dfs_for_connectivity(0);
    for (integer i = 0; i < n_; ++i) {
        if (!_visited_[i]) {
            return false;
        }
    }
    return true;
}

void Graph::_dfs_for_connectivity(integer s) {
    for (auto &t: V_[s].neighbors_) {
        if (!_visited_[t]) {
            _visited_[t] = true;
            _dfs_for_connectivity(t);
        }
    }
}


void Graph::enumerate_connected_components() {
    connected_components_.clear();
    if (n_ == 0) {
        return;
    }
    for (integer i = 0; i < n_; ++i) {
        _visited_[i] = false;
    }
    for (integer start = 0; start < n_; ++start) {
        if (_visited_[start]) {
            continue; // already counted as connected component
        }
        _visited_[start] = true;
        connected_components_.push_back({start});
        _dfs_for_connected_components(start);
    }
}

void Graph::_dfs_for_connected_components(integer s) {
    for (auto &t: V_[s].neighbors_) {
        if (!_visited_[t]) {
            _visited_[t] = true;
            connected_components_.back().push_back(t);
            _dfs_for_connected_components(t);
        }
    }
}

void Graph::enumerate_two_edge_connected_components() {
    two_edge_connected_components_.clear();
    if (n_ == 0) {
        return;
    }
    for (integer i = 0; i < n_; ++i) {
        _removed_bridges_[i].clear();
    }
    enumearte_bridges();
    for (auto &bridge: bridges_) {
        auto [u, v] = bridge;
        _removed_bridges_[u].insert(v);
        _removed_bridges_[v].insert(u);
    }

    for (integer i = 0; i < n_; ++i) {
        _visited_[i] = false;
    }
    for (integer start = 0; start < n_; ++start) {
        if (_visited_[start]) {
            continue; // already counted as 2-edge-connected component
        }
        _visited_[start] = true;
        bitset<N> two_edge_connected_component;
        two_edge_connected_component.set(start);
        two_edge_connected_components_.push_back(two_edge_connected_component);
        _dfs_for_two_edge_connected_components(start);
    }
}

void Graph::_dfs_for_two_edge_connected_components(integer s) {
    for (auto &t: V_[s].neighbors_) {
        if (!_visited_[t] && _removed_bridges_[s].find(t) == _removed_bridges_[s].end()) {
            _visited_[t] = true;
            two_edge_connected_components_.back().set(t);
            _dfs_for_two_edge_connected_components(t);
        }
    }
}

void Graph::enumerate_two_vertex_connected_components() {
    // if we find an articulation point in algorithm for enumerating art. points,
    // then the descendants and the point is maximal 2-vertex-conected component
    // since 2-vertex-connected components cannot be cycle.
    two_vertex_connected_components_.clear();
    if (n_ == 0) {
        return;
    }
    _compute_order_and_lowpt();

    for (integer i = 0; i < n_; ++i) {
        _visited_[i] = false;
    }
    for (integer i = 0; i < n_; ++i) {
        if (_visited_[i]) {
            continue;
        }
        _visited_[i] = true;
        auto _stack = _dfs_for_two_vertex_connected_components(i);
        (void)_stack; // root is already safisying the condition of art. point
    }
}

bitset<N> Graph::_dfs_for_two_vertex_connected_components(integer s) {
   bitset<N> component;
   for (auto &t : V_[s].neighbors_) {
        if (!_visited_[t]) {
            _visited_[t] = true;
            auto stack = _dfs_for_two_vertex_connected_components(t);
            if (_lowpt_[t] >= _order_[s]) {
                // find 2-vertex-connected component
                stack.set(s);
                two_vertex_connected_components_.push_back(stack);
            } else {
                component |= stack;
            }
        }
   }
   component.set(s);
   return component;
}

void Graph::enumearte_bridges() {
    bridges_.clear();
    if (n_ == 0) {
        return;
    }
    _compute_order_and_lowpt();

    // edge {u, v} is a bridge iff
    // 1. (u, v) is an arc of DFS tree; and
    // 2. order[u] < lowpt[v];
    for (integer i = 0; i < n_; ++i) {
        _visited_[i] = false;
    }
    for (integer i = 0; i < n_; ++i) {
        if(_visited_[i]) {
            continue;
        }
        _visited_[i] = true;
        _enumerate_bridges_inner(i);
    }
}

void Graph::_enumerate_bridges_inner(integer s) {
    for (auto &t: V_[s].neighbors_) {
        if (!_visited_[t]) {
            _visited_[t] = true;
            if (_order_[s] < _lowpt_[t]) {
                bridges_.push_back({std::min(s, t), std::max(s, t)});
            }
            _enumerate_bridges_inner(t);
        }
    }
}

void Graph::enumerate_articulation_points() {
    articulation_points_.clear();
    if (n_ == 0) {
        return;
    }
    _compute_order_and_lowpt();

    // v is an articulation point iff
    // 1. v is root and v has greater than two children; or
    // 2. v is not root and there is a child u s.t. lowpt[u] >= order[v];
    for (integer i = 0; i < n_; ++i) {
        _visited_[i] = false;
    }
    for (integer i = 0; i < n_; ++i) {
        if (_visited_[i]) {
            continue;
        }
        integer num_children = 0;
        _visited_[i] = true;
        _enumerate_articulation_points_inner(i, num_children, i);
        if (num_children > 1) {
            articulation_points_.push_back(i);
        }
    }
}

void Graph::_enumerate_articulation_points_inner(integer s, integer& num_children, integer& root) {
    for (auto &t: V_[s].neighbors_) {
        if (!_visited_[t]) {
            if (s == root) {
                num_children += 1;
            } else if (_lowpt_[t] >= _order_[s]) {
                articulation_points_.push_back(s);
            }
            _visited_[t] = true;
            _enumerate_articulation_points_inner(t, num_children, root);
        }
    }
}

void Graph::_compute_order_and_lowpt() {
    // compute order
    for (integer i = 0; i < n_; ++i) {
        _visited_[i] = false;
        _order_[i] = 0;
    }
    for (integer i = 0; i < n_; ++i) {
        if (_visited_[i]) {
            continue;
        }
        integer num = 0;
        _visited_[i] = true;
        _dfs_for_order(i, num);
    }

    // compute lowpt
    for (integer i = 0; i < n_; ++i) {
        _visited_[i] = false;
        _lowpt_[i] = _order_[i];
    }
    for (integer i = 0; i < n_; ++i) {
        if (_visited_[i]) {
            continue;
        }
        _visited_[i] = true;
        _dfs_for_lowpt(i, -1);
    }
}

void Graph::_dfs_for_order(integer s, integer& num) {
    _order_[s] = num;
    for (auto &t : V_[s].neighbors_) {
        if (!_visited_[t]) {
            _visited_[t] = true;
            num += 1;
            _dfs_for_order(t, num);
        }
    }
}

void Graph::_dfs_for_lowpt(integer s, integer parent) {
    for (auto &t: V_[s].neighbors_) {
        if (!_visited_[t]) {
            // forward edge
            _visited_[t] = true;
            _dfs_for_lowpt(t, s);
            _lowpt_[s] = std::min(_lowpt_[s], _lowpt_[t]);
        } else {
            if (t == parent) {
                continue;
            }
            // backword edge
            _lowpt_[s] = std::min(_lowpt_[s], _order_[t]);
        }
    }
}

Graph graph::generateGraphFromFile(string &&filename) {
    ifstream file;
    file.open(filename, ios::in);
    string buffer;
    stringstream ss;
    integer line = 0;

    auto vmap = unordered_map<integer, integer>();
    auto vmap_inv = unordered_map<integer, integer>();
    auto vname = vector<integer>();
    integer id = 0;
    integer m = 0;

    vector<Vertex> V;

    while (getline(file, buffer)) {
        line++;
        if (buffer.starts_with("#")) {
            continue; // comment
        }

        ss.clear();
        ss.str("");
        ss << buffer;
        integer u, w, v;
        ss >> u >> w >> v;

        if (u == v) {
            cerr << "ERROR: The line " << line << " is ignored since "
                << u << " = " << v << " (self-loop)" << endl;
        }

        assignIdIfNotExists(vmap, vmap_inv, vname, id, u, V);
        assignIdIfNotExists(vmap, vmap_inv, vname, id, v, V);

        auto u_id = vmap.find(u)->second;
        auto v_id = vmap.find(v)->second;
        V[u_id].neighbors_.push_back(v_id);
        V[u_id].deg_++;
        m++;
    }
    file.close();

    return Graph(id, m, std::move(V), std::move(vmap), std::move(vmap_inv), std::move(vname));
}

Graph Graph::generateInducedGraphFromBitset(bitset<N> &C) {
    #ifdef __DEBUG_
        cout << "TRACE: Graph::generateInducedGraphFromBitset" << endl;
        OutputBitsetForDebug(C);
    #endif

    auto vmap = unordered_map<integer, integer>();
    auto vmap_inv = unordered_map<integer, integer>();
    auto vname = vector<integer>();
    integer id = 0;
    integer m = 0;
    vector<Vertex> V;

    for (integer u = 0; u < n_; ++u) {
        if (!C[u]) {
            continue;
        }
        assignIdIfNotExists(vmap, vmap_inv, vname, id, u, V);
        auto u_id = vmap.find(u)->second;
        for (auto &v : V_[u].neighbors_) {
            if (!C[v]) {
                continue;
            }
            assignIdIfNotExists(vmap, vmap_inv, vname, id, v, V);
            auto v_id = vmap.find(v)->second;
            V[u_id].neighbors_.push_back(v_id);
            V[u_id].deg_++;
            m++;
        }
    }

    auto g = Graph(id, m, std::move(V), std::move(vmap), std::move(vmap_inv), std::move(vname));
    #ifdef __DEBUG_
        cout << "TRACE: Graph::generateInducedGraphFromBitset<" << endl;
        for (auto& v: g.V_) {
        cout << "id: " << v.id_ << ", deg: " << v.deg_ << endl;
        cout << "neighbors: ";
        for (auto& v: v.neighbors_) {
            cout << v << ",";
        }
        cout << endl;
    }
    #endif
    return g;
}

void Graph::enumerate_cut_point_pairs() {
    #ifdef __DEBUG_
        cout << "TRACE: Graph::enumerate_cut_point_pairs" << endl;
    #endif
    cut_point_pairs_.clear();
    // TODO: Implement
}

void assignIdIfNotExists(unordered_map<integer, integer> &vmap,
    unordered_map<integer, integer> &vmap_inv, vector<integer> &vname,
    integer &id, integer vertex_name, vector<Vertex> &V) {
    if (!vmap.contains(vertex_name)) {
        vmap.emplace(vertex_name, id);
        vmap_inv.emplace(id, vertex_name);
        vname.push_back(vertex_name);
        V.push_back(graph::Vertex());
        V[id].id_ = id;
        V[id].deg_ = 0;
        id++;
    }
}

void Graph::output_relations_between_name_and_id() {
    cout << "name\t|\tid" << endl;
    for (auto &kv : vmap_) {
        auto [name, id] = kv;
        cout << name << "\t|\t" << id << endl;
    }
}

std::bitset<N> Graph::inverse_bitset(std::bitset<N> B) {
    bitset<N> C;
    for(integer i = 0; i < N; ++i) {
        if (B[i]) {
            C.set(vmap_inv_[i]);
        }
    }
    return C;
}