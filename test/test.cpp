#include <iostream>
#include <vector>
#include <tuple>
#include "../src/lib/lib.hpp"
#include "../src/lib/power.hpp"
#include "../src/lib/graph.hpp"
using namespace std;

void power();
void low_link();

int main() {
    cout << "Test:\n" << endl;
    power();
    low_link();
}

void power() {
    cout << "Power:" << endl;
    // where
    static_assert(SubsetDisjoint<PowerSet>);
    PowerSet p1, p2;
    auto system1 = SDSystem(std::move(p1));
    auto system2 = SDSystem(std::move(p2));

    auto C1 = bitset<N>();
    auto I1 = C1;
    C1.flip();
    auto C2 = bitset<N>();
    auto I2 = C2;
    C2.flip();

    // when
    cout << "enumerate with list_simple" << endl;
    system1.list_simple(C1, I1);
    cout << "enumerate with list" << endl;
    system2.list(C2, I2);

    // then
    cout << "then" << endl;
    vector<unsigned long long> answer1, answer2, solutions;

    for (auto&e: system1.system_.solutions_) {
        answer1.emplace_back(e.to_ullong());
    }
    for (auto&e: system2.system_.solutions_) {
        answer2.emplace_back(e.to_ullong());
    }
    for (integer t = 0; t < ((integer)1 << N); ++t) {
        solutions.emplace_back((unsigned long long)t);
    }
    sort(answer1.begin(), answer1.end());
    sort(answer2.begin(), answer2.end());
    sort(solutions.begin(), solutions.end());

    assert(answer1 == answer2);
    cout << answer1.size() << ", " << solutions.size() << endl;
    assert(answer1 == solutions);
    cout << endl;
}

void low_link() {
    cout << "2-edge/vertex-connectivity:" << endl;
    // -- Given --
    graph::Graph g(11);
    g.edges_.adjacency_[0].push_back(1);
    g.edges_.adjacency_[0].push_back(2);
    g.edges_.adjacency_[0].push_back(3);
    g.edges_.adjacency_[1].push_back(0);
    g.edges_.adjacency_[1].push_back(4);
    g.edges_.adjacency_[1].push_back(5);
    g.edges_.adjacency_[2].push_back(0);
    g.edges_.adjacency_[2].push_back(3);
    g.edges_.adjacency_[3].push_back(0);
    g.edges_.adjacency_[3].push_back(2);
    g.edges_.adjacency_[3].push_back(5);
    g.edges_.adjacency_[3].push_back(6);
    g.edges_.adjacency_[4].push_back(1);
    g.edges_.adjacency_[4].push_back(6);
    g.edges_.adjacency_[5].push_back(1);
    g.edges_.adjacency_[5].push_back(3);
    g.edges_.adjacency_[6].push_back(3);
    g.edges_.adjacency_[6].push_back(4);
    g.edges_.adjacency_[6].push_back(7);
    g.edges_.adjacency_[6].push_back(8);
    g.edges_.adjacency_[7].push_back(6);
    g.edges_.adjacency_[7].push_back(9);
    g.edges_.adjacency_[8].push_back(6);
    g.edges_.adjacency_[8].push_back(9);
    g.edges_.adjacency_[9].push_back(7);
    g.edges_.adjacency_[9].push_back(8);
    g.edges_.adjacency_[9].push_back(10);
    g.edges_.adjacency_[10].push_back(9);

    // -- When --
    g.enumerate_articulation_points();
    g.enumearte_bridges();

    // -- Then --
    std::vector<integer> expected_art({ 6, 9 });
    assert(g.articulation_points_ == expected_art);
    std::vector<std::tuple<integer, integer>> expected_bri({ { 9, 10 } });
    assert(g.bridges_ == expected_bri);

    cout << "ok\n" << endl;
}