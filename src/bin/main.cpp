#include <iostream>
#include <bitset>
#include <set>
#include <vector>

#include "../lib/lib.hpp"
#include "../lib/power.hpp"
#include "../lib/graph.hpp"
#include "../lib/util.hpp"
#include "../lib/two-edge-connected.hpp"
#include "../lib/two-edge-connected-square.hpp"
#include "../lib/two-vertex-connected.hpp"
#include "../lib/two-vertex-connected-square.hpp"
#include "../lib/cpu_time.hpp"

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/decomposition/StaticSPQRTree.h>

/**
 * \file main.cpp
 * \brief Test code
 * \author T.Tada,
*/
void addEdge(ogdf::Graph &G, std::vector<ogdf::node> &node_container, integer u, integer v);
void addAllEdges(ogdf::Graph &G, std::vector<ogdf::node> &node_container);

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace graph;

    auto param = readArgs(argc, argv);
    cout << "input:\t" << param.input_file_name_ << '\n'
         << "output:\t" << param.outname_ << '\n'
         << "common:\t" << param.common_outname_ << '\n'
         << "type:\t" << param.exp_type_ << '\n'
         << "tlime:\t" << param.tlime_ << endl;

    integer numCount = 0;
    double duration = 0;
    bool is_fisnished = false;

    if (param.exp_type_ == 0) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystem(TwoEdgeConnectedSquare(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_edge_connected_components();
        for (auto &C: system.system_.given_.two_edge_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 1) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystem(TwoEdgeConnectedSquare(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_edge_connected_components();
        for (auto &C: system.system_.given_.two_edge_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list_simple(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 2) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystemWithEnumOracle(TwoEdgeConnectedSquare(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_edge_connected_components();
        for (auto &C: system.system_.given_.two_edge_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list_simple(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 3) {
       graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystem(TwoEdgeConnected(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_edge_connected_components();
        for (auto &C: system.system_.given_.two_edge_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 4) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystem(TwoEdgeConnected(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_edge_connected_components();
        for (auto &C: system.system_.given_.two_edge_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list_simple(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 5) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystemWithEnumOracle(TwoEdgeConnected(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_edge_connected_components();
        for (auto &C: system.system_.given_.two_edge_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list_simple(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 6) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystem(TwoVertexConnectedSquare(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_vertex_connected_components();
        for (auto &C: system.system_.given_.two_vertex_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 7) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystem(TwoVertexConnectedSquare(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_vertex_connected_components();
        for (auto &C: system.system_.given_.two_vertex_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list_simple(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 8) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystemWithEnumOracle(TwoVertexConnectedSquare(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_vertex_connected_components();
        for (auto &C: system.system_.given_.two_vertex_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list_simple(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 9) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystem(TwoVertexConnected(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_vertex_connected_components();
        for (auto &C: system.system_.given_.two_vertex_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 10) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystem(TwoVertexConnected(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_vertex_connected_components();
        for (auto &C: system.system_.given_.two_vertex_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list_simple(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 11) {
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        auto system = SDSystemWithEnumOracle(TwoVertexConnected(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_vertex_connected_components();
        for (auto &C: system.system_.given_.two_vertex_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list_simple(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    } else if (param.exp_type_ == 12) {
        // for debug
        graph::Graph g = generateGraphFromFile(std::move(param.input_file_name_));
        g.output_relations_between_name_and_id();
        g.enumerate_articulation_points();
        cout << g.articulation_points_.size() << endl;
        for(auto& e: g.articulation_points_) {
            cout << e << endl;
        }
        g.enumerate_two_vertex_connected_components();
        for(auto &C: g.two_vertex_connected_components_) {
            OutputBitsetForDebug(C);
        }
        auto system = SDSystem(TwoVertexConnectedSquare(g), param.tlime_);
        cout << "Start to enumerate.: " << param.input_file_name_ << endl;
        auto sd_start = cpu_time();
        system.system_.given_.enumerate_two_vertex_connected_components();
        for (auto &C: system.system_.given_.two_vertex_connected_components_) {
            if (C.count() < 3) {
                continue;
            }
            auto I = bitset<N>(); // emptyset
            system.list(C, I);
        }
        auto sd_end = cpu_time();
        duration = sd_end - sd_start;
        numCount = system.system_.num_count_;
        is_fisnished = !system.timeout_;
        cout << "CPU time of list: " << duration << endl;
        cout << "numCount: " << numCount << endl;
    }

    writeResults(param, numCount, duration, is_fisnished);

}

void addEdge(ogdf::Graph &G, std::vector<ogdf::node> &node_container, integer u, integer v) {
    G.newEdge(node_container[u], node_container[v]);
    // G.newEdge(node_container[v], node_container[u]);
}

void addAllEdges(ogdf::Graph &G, std::vector<ogdf::node> &node_container) {
    addEdge(G, node_container, 0, 1);
    addEdge(G, node_container, 0, 2);
    addEdge(G, node_container, 0, 3);
    addEdge(G, node_container, 1, 4);
    addEdge(G, node_container, 1, 5);
    addEdge(G, node_container, 2, 4);
    addEdge(G, node_container, 2, 6);
    addEdge(G, node_container, 3, 6);
    addEdge(G, node_container, 3, 7);
    addEdge(G, node_container, 4, 8);
    addEdge(G, node_container, 5, 9);
    addEdge(G, node_container, 5, 10);
    addEdge(G, node_container, 5, 11);
    addEdge(G, node_container, 6, 8);
    addEdge(G, node_container, 7, 8);
    addEdge(G, node_container, 7, 12);
    addEdge(G, node_container, 7, 13);
    addEdge(G, node_container, 9, 10);
    addEdge(G, node_container, 9, 11);
    addEdge(G, node_container, 9, 14);
    addEdge(G, node_container, 9, 15);
    addEdge(G, node_container, 10, 11);
    addEdge(G, node_container, 12, 13);
    addEdge(G, node_container, 12, 15);
    addEdge(G, node_container, 13, 14);
    addEdge(G, node_container, 14, 15);
}