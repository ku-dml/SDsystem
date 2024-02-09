#include "lib.hpp"
using namespace std;

void OutputBitsetForDebug(bitset<N>& B) {
    cout << "{";
    integer i = 0;
    for (; i < N; ++i) { // find first element
        if (B[i]) {
            cout << i;
            ++i;
            break;
        }
    }
    for (; i < N; ++i) { // find the rest
        if (B[i]) {
            cout << "," << i;
        }
    }
    cout << "}" << endl;
}

void convertBitsetToVector(std::bitset<N> &bits, std::vector<integer> &vec) {
    vec.clear();
    for (integer i = 0; i < bits.size(); ++i) {
        if (bits.test(i)) {
            vec.push_back(i);
        }
    }
}

void convertVectorToBitset(std::vector<integer> &vec, std::bitset<N>& bits) {
    bits.reset();
    for (auto &e : vec) {
        bits.set(e);
    }
}

void setMinus(std::bitset<N> &left, std::bitset<N> &right) {
    right.flip();
    left &= right;
    right.flip();
}

void setMinusVector(std::bitset<N> &left, std::vector<integer> &right) {
    for (auto& e: right) {
        left.reset(e);
    }
}