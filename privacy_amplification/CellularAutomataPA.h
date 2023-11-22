//
// Created by marco on 07/07/23.
//

#ifndef PRIVACYAMPLIFICATIONCPP_CELLULARAUTOMATAPA_H
#define PRIVACYAMPLIFICATIONCPP_CELLULARAUTOMATAPA_H


#include <vector>
#include "Key.h"
#include <bits/stdc++.h>
#include <array>
#include <cmath>
#include <bitset>

class CellularAutomataPA {
    public:
        static uint64_t rule(int left_cell, int center_cell, int right_cell);
        static std::vector<uint64_t> update(const std::vector<uint64_t> &CA, int bits);
        static std::vector<std::vector<uint64_t>> splitVector(std::vector<uint64_t> words, int requiredBits, int actualBits);
        static std::vector<uint64_t> bitwise_and(std::vector<uint64_t> &T_block, std::vector<uint64_t> &CA);
        static std::vector<uint64_t> bitwise_xor(std::vector<uint64_t> &accumulator, std::vector<uint64_t> &ands);
        static Key Amplificate(Key Key, int K, int M, int N, std::vector<uint64_t>& initial_value);
        static std::vector<uint64_t> extract_bits(const std::vector<uint64_t>& data, int start_index, int end_index);
        static std::vector<uint64_t> combine_bits(std::vector<uint64_t> *data, int numberOfBits);
};


#endif //PRIVACYAMPLIFICATIONCPP_CELLULARAUTOMATAPA_H
