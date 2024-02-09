#ifndef __LIB_HPP_
#define __LIB_HPP_

// #define __DEBUG_

#include <bitset>
#include <optional>
#include <vector>
#include <array>
#include <iostream>

#include "cpu_time.hpp"

#include <stdint.h>
#define integer int64_t

// constexpr integer N = 4000;
constexpr integer N = 12000;

#define INI_tlim -1.0
#define INI_outname "out.txt"
#define INI_common_outname "results.csv"

void convertBitsetToVector(std::bitset<N> &bits, std::vector<integer> &vec);
void convertVectorToBitset(std::vector<integer> &vec, std::bitset<N>& bits);
void setMinus(std::bitset<N> &left, std::bitset<N> &right);
void setMinusVector(std::bitset<N> &left, std::vector<integer> &right);
void OutputBitsetForDebug(std::bitset<N>& B);

// Set system satisfying Subset-Disjoint property
template <class System>
concept SubsetDisjoint = requires(System &system, std::bitset<N> &C, std::bitset<N> &X) {
    // output solution
    { system.output(C) } -> std::convertible_to<void>;
    // compute MRS Y of C that is disjoint with X
    { system.computeMrs(C, X) } -> std::convertible_to<std::optional<std::bitset<N>>>;
};

template <class System>
concept SubsetDisjointWithEnumOracle = requires(System &system, std::bitset<N> &C, std::bitset<N> &X) {
    // output solution
    { system.output(C) } -> std::convertible_to<void>;
    // compute All MRS of C that is disjoint with X
    { system.computeAllMrs(C, X) } -> std::convertible_to<std::vector<std::bitset<N>>>;
};

template <SubsetDisjoint System>
class SDSystem {
public:
    System system_;
    bool timeout_;

private:
    // C
    std::bitset<N> component_;
    // X
    std::bitset<N> x_;
    std::array<std::vector<integer>, N + 1> x_manager_;
    integer x_last_index_;
    // Y
    std::optional<std::bitset<N>> mrs_;
    // Seq
    std::array<std::vector<integer>, N + 1> sequence_;
    integer seq_last_index_;
    // Size
    std::array<integer, N + 1> size_;
    integer size_last_index_;
    // for efficient allocation
    std::bitset<N> seq_last_bits_;
    double _start_time_;
    double _time_limit_;

public:
    explicit SDSystem(System &&system, double time_limit)
        : system_ { system }
        , timeout_(false)
        , component_ { {} }
        , x_ { {} }
        , x_last_index_ { -1 }
        , mrs_ { std::nullopt }
        , seq_last_index_ { -1 }
        , size_last_index_ { -1 }
        , seq_last_bits_ { {} }
        , _time_limit_(time_limit)
        {
            for (auto& v: x_manager_) {
                v.clear();
                v.reserve(N);
            }
            for (auto& v: sequence_) {
                v.clear();
                v.reserve(N);
            }
            for (auto& e: size_) {
                e = 0;
            }
        }
    // enumerate solution C' \subseteq C s.t. I \subseteq C'
    // O(n + \theta_t) delay, O(n^2 + \theta_s) space
    void list_simple(std::bitset<N> &C, std::bitset<N> &I) {
        _start_time_ = cpu_time();
        _list_simple_inner(C, I, 0);
    }
    // enumerate solution C' \subseteq C s.t. I \subseteq C'
    // O(n + \theta_t) delay, O(n + \theta_s) space
    void list(std::bitset<N> &C, std::bitset<N> &I) {
        // initialization
        component_ = std::move(C);
        x_ = std::move(I);
        integer depth = 0;
        _start_time_ = cpu_time();

        while (true) {
            if (_time_limit_ != -1.0 && cpu_time() - _start_time_ >= _time_limit_) {
                timeout_ = true;
                break;
            }
            // alternative output
            if (depth % 2 == 1 && size_[depth] == 0) {
                system_.output(component_);
            }
            mrs_ = system_.computeMrs(component_, x_);
            if (mrs_.has_value()) {
                // Seq.push(Y);
                _seq_push_back(mrs_.value());
                // C' := C' \ Y;
                setMinus(component_, mrs_.value());
                size_[depth]++;
                depth++;
            } else {
                // alternative output
                if (depth % 2 == 0) {
                    system_.output(component_);
                    if (depth == 0) {
                        break;
                    }
                }
                convertVectorToBitset(sequence_[seq_last_index_], seq_last_bits_);
                // C' := C' U Seq.last();
                component_ |= seq_last_bits_;
                // X.shrinkTo(X.length() - Siz[depth]);
                _x_shrink(size_[depth]);
                size_[depth] = 0;
                // X.push(Seq.last());
                _x_push_back(seq_last_bits_, sequence_[seq_last_index_]);
                // Seq.pop();
                seq_last_index_--;
                depth--;
            }
        }
    }

private:
    void _list_simple_inner(std::bitset<N> &C, std::bitset<N> &I, integer depth) {
        if (_time_limit_ != -1.0 && cpu_time() - _start_time_ > _time_limit_) {
            timeout_ = true;
            return;
        }
        if (depth % 2 == 0) {
            system_.output(C);
        }
        std::bitset<N> X = I;
        auto Y = system_.computeMrs(C, X);
        while (Y.has_value()) {
            // C' := C \ Y
            auto C_prime = C;
            setMinus(C_prime, Y.value());
            // list(C\Y, X)
            _list_simple_inner(C_prime, X, depth + 1);
            // X := X U Y
            X |= Y.value();
            // computeMRS
            Y = system_.computeMrs(C, X);
        }
        if (depth % 2 != 0) {
            system_.output(C);
        }
    }

    void _seq_push_back(std::bitset<N>& mrs) {
        convertBitsetToVector(mrs, sequence_[seq_last_index_ + 1]);
        seq_last_index_++;
    }

    void _x_push_back(std::bitset<N>& bits, std::vector<integer>& vec) {
        x_ |= bits;
        x_manager_[x_last_index_ + 1].clear();
        for (auto &e: vec) {
            x_manager_[x_last_index_ + 1].push_back(e);
        }
        x_last_index_++;
    }

    void _x_shrink(integer size) {
        // shrink x_manager_
        x_last_index_ -= size;
        // shrink x_
        for (integer i = 0; i < size; ++i) {
            setMinusVector(x_, x_manager_[x_last_index_ + 1 + i]);
        }
    }
};

template <SubsetDisjointWithEnumOracle System>
class SDSystemWithEnumOracle {
    public:
        System system_;
        bool timeout_;
    private:
        double _time_limit_;
        double _start_time_;

    public:
        explicit SDSystemWithEnumOracle(System &&system, double time_limit)
            : system_ { system }
            , timeout_(false)
            , _time_limit_(time_limit)
        {}

        void list_simple(std::bitset<N> &C, std::bitset<N> &I) {
        _start_time_ = cpu_time();
        _list_simple_inner(C, I, 0);
        }

    private:
        void _list_simple_inner(std::bitset<N> &C, std::bitset<N> &I, integer depth) {
            if (_time_limit_ != 1.0 && cpu_time() - _start_time_ > _time_limit_) {
                timeout_ = true;
                return;
            }
            if (depth % 2 == 0) {
                system_.output(C);
            }
            std::bitset<N> X = I;
            auto MRSs = system_.computeAllMrs(C, X);
            for (auto &Y: MRSs) {
                auto C_prime = C;
                // C' := C \ Y
                setMinus(C_prime, Y);
                // list(C\Y, X)
                _list_simple_inner(C_prime, X, depth + 1);
                // X := X ∪ Y
                X |= Y;
            }
            if (depth % 2 == 1) {
                system_.output(C);
            }
        }
};

#endif