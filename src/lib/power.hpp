#ifndef __POWER_HPP_
#define __POWER_HPP_
#include "lib.hpp"
#include <string>
#include <vector>

// enumerate power set
class PowerSet {
    std::bitset<N> candidate_;
    std::bitset<N> Y_;
    public:
        std::vector<std::bitset<N>> solutions_;

public:
    explicit PowerSet() {};
    void output(std::bitset<N> &C);
    std::optional<std::bitset<N>> computeMrs(std::bitset<N> &C, std::bitset<N> &X);
};

#endif