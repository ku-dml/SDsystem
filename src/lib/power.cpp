#include "power.hpp"
#include <iostream>

void PowerSet::output(std::bitset<N>& C) {
    // std::cout << "Solution: " << C << std::endl;
    solutions_.emplace_back(C);
}

std::optional<std::bitset<N>> PowerSet::computeMrs(std::bitset<N> &C, std::bitset<N> &X) {
    X.flip();
    candidate_ = C & X;
    X.flip();
    for(int32_t i = 0; i < C.size(); ++i) {
        Y_.reset();
        Y_.set(i);
        if ((candidate_ & Y_).count() != 0) {
            return Y_;
        }
    }
    return std::nullopt;
}