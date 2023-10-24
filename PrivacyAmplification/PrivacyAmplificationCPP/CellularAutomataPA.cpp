//
// Created by marco on 07/07/23.
//

#include "CellularAutomataPA.h"
static uint64_t end_word_mask(int end_bit_nr)
{
    uint64_t mask = 0xffffffffffffffffull;

    int nr_unused_bits = 64 - ((end_bit_nr) % 64);
    if (nr_unused_bits != 64)
        mask >>= nr_unused_bits;
    return mask;
}


uint64_t CellularAutomataPA::rule(int left_cell, int center_cell, int right_cell) {
    int rule_number = 150;
    std::bitset<8> ruleN(rule_number);
    int index = (left_cell << 2) | (center_cell << 1) | right_cell;

    return ruleN[7-index];
}



std::vector<uint64_t> CellularAutomataPA::update(const std::vector<uint64_t>& CA, int bits) {
    int reminder = bits % 64;
    std::vector<uint64_t> new_CA(CA.size(),0);
    uint64_t  mask=0x0000000000000001;
    if(CA.size()==1) {
        for (int j = 0; j < (reminder ? reminder : 64); j++) {
            if (j == 0) {
                uint64_t center = CA[0] >> j & (mask);
                uint64_t left = CA[0] >> (reminder ? reminder-1 : 63) & (mask);
                uint64_t right = CA[0] >> (j + 1) & (mask);
                new_CA[0] ^= (rule(left, center, right) << j);
            } else if (j == (reminder ? reminder-1 : 63)) {
                uint64_t center = CA[0] >> j & (mask);
                uint64_t left = CA[0] >> (j - 1) & (mask);
                uint64_t right = CA[0] & (mask);
                new_CA[0] ^= (rule(left, center, right) << j);
            } else {
                uint64_t center = CA[0] >> j & (mask);
                uint64_t left = CA[0] >> (j - 1) & (mask);
                uint64_t right = CA[0] >> (j + 1) & (mask);
                new_CA[0] ^= (rule(left, center, right) << j);
            }
        }
        return new_CA;
    }
    for (int i = 0; i < CA.size(); i++) {
        if (i == 0) {
            for(int j=0;j<64;j++){
                if(j==0){
                    uint64_t center=CA[i]>>j & (mask);
                    uint64_t left=CA[CA.size()-1]>> (reminder? reminder-1:63) & (mask);
                    uint64_t right=CA[i]>>(j+1) & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                }
                else if (j==63){
                    uint64_t center=CA[i]>>j & (mask );
                    uint64_t left=CA[i]>>(j-1) & (mask );
                    uint64_t right=CA[i+1]>>0 & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                }
                else{
                    uint64_t center=CA[i]>>j & (mask);
                    uint64_t left=CA[i]>>(j-1) & (mask );
                    uint64_t right=CA[i]>>(j+1) & (mask );
                    new_CA[i] ^= rule(left, center, right) << j;
                }
            }
            continue;
        }
        if (i == CA.size() - 1) {
            for(int j=0;j<(reminder? reminder:64);j++){
                if(j==0){
                    uint64_t center=CA[i]>>j & (mask );
                    uint64_t left=CA[i-1]>>63 & (mask );
                    uint64_t right=CA[i]>>(j+1) & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                }
                else if (j==(reminder? reminder:63)){
                    uint64_t center=CA[i]>>j & (mask );
                    uint64_t left=CA[i]>>(j-1) & (mask);
                    uint64_t right=CA[0] & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                }
                else{
                    uint64_t center=CA[i]>>j & (mask );
                    uint64_t left=CA[i]>>(j-1) & (mask );
                    uint64_t right=CA[i]>>(j+1) & (mask );
                    new_CA[i] ^= rule(left, center, right) << j;
                }
            }
            continue;
        }
        for(int j=0;j<64;j++){
            if(j==0){
                uint64_t center=CA[i]>>j & (mask );
                uint64_t left=CA[i-1]>>63 & (mask );
                uint64_t right=CA[i]>>(j+1) & (mask);
                new_CA[i] ^= rule(left, center, right) << j;
            }
            else if (j==63){
                uint64_t center=CA[i]>>j & (mask );
                uint64_t left=CA[i]>>(j-1) & (mask );
                uint64_t right=CA[i+1] & (mask);
                new_CA[i] ^= rule(left, center, right) << j;
            }
            else{
                uint64_t center=CA[i]>>j & (mask );
                uint64_t left=CA[i]>>(j-1) & (mask );
                uint64_t right=CA[i]>>(j+1) & (mask );
                new_CA[i] ^= rule(left, center, right) << j;
            }
        }
    }

    return new_CA;
}


std::vector<std::vector<uint64_t>> CellularAutomataPA::splitVector(std::vector<uint64_t> words, int requiredBits, int actualBits) {
    std::vector<std::vector<uint64_t>> result(actualBits/requiredBits+ (actualBits%requiredBits? 1:0));
    for(auto &i:result){
        i.resize(requiredBits/64);
    }
    int remainder = actualBits % 64? actualBits % 64:64;
    int residualRequiredBits=requiredBits;
    int residualActualBits= words.size()==1 ? remainder : 64;
    int k=0;
    for(int j=0;j<actualBits;0){
        int transferedBits=residualRequiredBits<residualActualBits? residualRequiredBits:residualActualBits;
        result[k].push_back(words[j / 64] & (end_word_mask(transferedBits)));


        if(transferedBits!=64) {
            words[j / 64] = words[j / 64] >> transferedBits;

        }
        else {
            words[j / 64] = 0;
        }
        j+=transferedBits;
        residualActualBits-=transferedBits;
        residualRequiredBits-=transferedBits;
        if(residualActualBits==0){
            if(j/64==(words.size() - 1))
                residualActualBits=remainder;
            else
                residualActualBits=64;
        }
        if(residualRequiredBits==0){
            residualRequiredBits=requiredBits;
            k++;
        }
    }

    return result;
}



std::vector<uint64_t> CellularAutomataPA::bitwise_and(std::vector<uint64_t>& vec1, std::vector<uint64_t>& vec2) {
    std::vector<uint64_t> result;
    result.reserve(vec1.size());
for (int i = 0; i < vec1.size(); i++) {
        result.push_back(vec1[i] & vec2[i]);
    }
    return result;
}

std::vector<uint64_t> CellularAutomataPA::bitwise_xor(std::vector<uint64_t>& vec1, std::vector<uint64_t>& vec2) {
    std::vector<uint64_t> result;
    result.reserve(vec1.size());
for (int i = 0; i < vec1.size(); i++) {
        result.push_back(vec1[i] ^ vec2[i]);
    }
    return result;
}


Key CellularAutomataPA::Amplificate(Key key, int K, int M, int N, std::vector<uint64_t>& initial_value) {

    if (key.get_nr_bits() > M * K) {
        std::cout << "M*K cannot be less than key length" << std::endl;
        exit(-1);
    }

    // Step 1: Divide the received n bits negotiation key Key into small groups with length M
    std::vector<std::vector<uint64_t>> T_groups;
    if (key.get_nr_bits() < M * K) {
        // Add zeros to the last group if necessary
        key.addPadding((M * K) - key.get_nr_bits());
    }
    T_groups = splitVector(key.getWords(), M, key.get_nr_bits());

    // Step 2: Initialize CA and set its running rules
    std::vector<uint64_t> CA(initial_value);
    std::vector<uint64_t> accumulator(N/64+ (N%64? 1 : 0), 0);

    // Step 3: Compress each group negotiation key
    std::vector<uint64_t> final_key((K * N)/64+ ((K*N)%64? 1 : 0) , 0);
    for (int i = 0; i < K; i++) {
        std::vector<uint64_t> T_group = T_groups[i];
        std::vector<std::vector<uint64_t>> T_blocks = splitVector(T_group, N,M);
        for (auto T_block : T_blocks) {
            std::vector<uint64_t> ands = bitwise_and(T_block, CA);
            CA = update(CA,N);
            accumulator = bitwise_xor(accumulator, ands);
        }
        std::copy(accumulator.begin(), accumulator.end(), final_key.begin() + (i * N/64));
        std::copy(accumulator.begin(), accumulator.end(), CA.begin());
        CA=update(CA, N);
    }

    // Step 4: Combine the final key blocks into a final security key
    class Key PAKey (K*N,(K * N)/64+ ((K*N)%64? 1 : 0),final_key);

    return PAKey;
}