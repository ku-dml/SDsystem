#ifndef __GRAPH_HPP_
#define __GRAPH_HPP_

#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <unordered_map>

#include "lib.hpp"

namespace graph {
    class Vertex {
        public:
            integer id_; // generally, id_ is in [0, n_-1]
            std::vector<integer> neighbors_;
            integer deg_;
        public:
            Vertex(integer id, std::vector<integer> neighbors, integer deg)
            : id_(id), neighbors_(neighbors), deg_(deg) {}
            Vertex(): id_(0), neighbors_({}), deg_(0) {}
    };

    class Graph {
        public:
            std::vector<Vertex> V_; // adjacent list
            integer n_;
            integer m_;
            std::unordered_map<integer, integer> vmap_; // vertex name to vertex id
            std::unordered_map<integer, integer> vmap_inv_; // vertex id to vertex name
            std::vector<integer> vname_; // all vertex name
            std::vector<integer> articulation_points_;
            std::vector<std::tuple<integer, integer>> bridges_;
            std::vector<std::vector<integer>> connected_components_;
            std::vector<std::bitset<N>> two_edge_connected_components_;
            std::vector<std::bitset<N>> two_vertex_connected_components_;
            std::vector<std::tuple<integer, integer>> cut_point_pairs_;

        private:
            std::vector<bool> _visited_;
            std::vector<integer> _order_;
            std::vector<integer> _lowpt_;
            std::vector<std::tuple<integer, integer>> _bridges_;
            std::vector<std::set<integer>> _removed_bridges_;
        public:
            Graph(integer n, integer m, std::vector<Vertex> &&V, // required
                std::unordered_map<integer, integer> &&vmap, // optional
                std::unordered_map<integer, integer> &&vmap_inv, // optional
                std::vector<integer> &&vname) // optional
            : V_(V)
            , n_(n)
            , m_(m)
            , vmap_(vmap)
            , vmap_inv_(vmap_inv)
            , vname_(vname)
            , _visited_ (n, false)
            , _order_ (n, 0)
            , _lowpt_ (n, 0)
            , _removed_bridges_(n)
            { }
            // for undirected graph
            void enumerate_articulation_points();
            // for undirected graph
            void enumearte_bridges();
            // for undirected graph
            void enumerate_connected_components();
            // for undirected graph
            void enumerate_two_edge_connected_components();
            // for undirected graph
            void enumerate_two_vertex_connected_components();
            graph::Graph generateInducedGraphFromBitset(std::bitset<N> &C);
            void enumerate_cut_point_pairs();
            void output_relations_between_name_and_id();
            std::bitset<N> inverse_bitset(std::bitset<N> B);

            bool is_connected();
            bool is_cycle();
        private:
            void _dfs_for_connectivity(integer s);
            void _dfs_for_connected_components(integer s);
            void _dfs_for_two_edge_connected_components(integer s);
            std::bitset<N> _dfs_for_two_vertex_connected_components(integer s);
            void _compute_order_and_lowpt();
            void _dfs_for_order(integer s, integer &num);
            void _dfs_for_lowpt(integer s, integer parent);
            void _enumerate_articulation_points_inner(integer s, integer& num_children, integer& root);
            void _enumerate_bridges_inner(integer s);

    };


    // generate G from file
    Graph generateGraphFromFile(std::string &&filename);
}

#endif