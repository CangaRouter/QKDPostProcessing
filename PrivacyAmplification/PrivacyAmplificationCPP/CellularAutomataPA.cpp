//
// Created by marco on 07/07/23.
//

#include "CellularAutomataPA.h"

static uint64_t end_word_mask(int end_bit_nr) {
    uint64_t mask = 0xffffffffffffffffull;

    int nr_unused_bits = 64 - ((end_bit_nr) % 64);
    if (nr_unused_bits != 64)
        mask >>= nr_unused_bits;
    return mask;
}

static uint64_t start_word_mask(int start_bit_nr) {
    int nr_unused_bits = start_bit_nr % 64;
    return 0xffffffffffffffffull << nr_unused_bits;
}

uint64_t CellularAutomataPA::rule(int left_cell, int center_cell, int right_cell) {
    int rule_number = 150;
    std::bitset<8> ruleN(rule_number);
    int index = (left_cell << 2) | (center_cell << 1) | right_cell;

    return ruleN[7 - index];
}


std::vector<uint64_t> CellularAutomataPA::update(const std::vector<uint64_t> &CA, int bits) {
    int reminder = bits % 64;
    std::vector<uint64_t> new_CA(CA.size(), 0);
    uint64_t mask = 0x0000000000000001;
    if (CA.size() == 1) {
        for (int j = 0; j < (reminder ? reminder : 64); j++) {
            if (j == 0) {
                uint64_t center = CA[0] >> j & (mask);
                uint64_t left = CA[0] >> (reminder ? reminder - 1 : 63) & (mask);
                uint64_t right = CA[0] >> (j + 1) & (mask);
                new_CA[0] ^= (rule(left, center, right) << j);
            } else if (j == (reminder ? reminder - 1 : 63)) {
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
            for (int j = 0; j < 64; j++) {
                if (j == 0) {
                    uint64_t center = CA[i] >> j & (mask);
                    uint64_t left = CA[CA.size() - 1] >> (reminder ? reminder - 1 : 63) & (mask);
                    uint64_t right = CA[i] >> (j + 1) & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                } else if (j == 63) {
                    uint64_t center = CA[i] >> j & (mask);
                    uint64_t left = CA[i] >> (j - 1) & (mask);
                    uint64_t right = CA[i + 1] >> 0 & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                } else {
                    uint64_t center = CA[i] >> j & (mask);
                    uint64_t left = CA[i] >> (j - 1) & (mask);
                    uint64_t right = CA[i] >> (j + 1) & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                }
            }
            continue;
        }
        if (i == CA.size() - 1) {
            for (int j = 0; j < (reminder ? reminder : 64); j++) {
                if (j == 0) {
                    uint64_t center = CA[i] >> j & (mask);
                    uint64_t left = CA[i - 1] >> 63 & (mask);
                    uint64_t right = CA[i] >> (j + 1) & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                } else if (j == (reminder ? reminder : 63)) {
                    uint64_t center = CA[i] >> j & (mask);
                    uint64_t left = CA[i] >> (j - 1) & (mask);
                    uint64_t right = CA[0] & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                } else {
                    uint64_t center = CA[i] >> j & (mask);
                    uint64_t left = CA[i] >> (j - 1) & (mask);
                    uint64_t right = CA[i] >> (j + 1) & (mask);
                    new_CA[i] ^= rule(left, center, right) << j;
                }
            }
            continue;
        }
        for (int j = 0; j < 64; j++) {
            if (j == 0) {
                uint64_t center = CA[i] >> j & (mask);
                uint64_t left = CA[i - 1] >> 63 & (mask);
                uint64_t right = CA[i] >> (j + 1) & (mask);
                new_CA[i] ^= rule(left, center, right) << j;
            } else if (j == 63) {
                uint64_t center = CA[i] >> j & (mask);
                uint64_t left = CA[i] >> (j - 1) & (mask);
                uint64_t right = CA[i + 1] & (mask);
                new_CA[i] ^= rule(left, center, right) << j;
            } else {
                uint64_t center = CA[i] >> j & (mask);
                uint64_t left = CA[i] >> (j - 1) & (mask);
                uint64_t right = CA[i] >> (j + 1) & (mask);
                new_CA[i] ^= rule(left, center, right) << j;
            }
        }
    }

    return new_CA;
}


std::vector<std::vector<uint64_t>>
CellularAutomataPA::splitVector(std::vector<uint64_t> words, int requiredBits, int actualBits) {
    std::vector<std::vector<uint64_t>> result(actualBits / requiredBits + (actualBits % requiredBits ? 1 : 0));
    int j = 0;
    for (int i = 0; i < result.size(); i++) {
        if (i == result.size() - 1) {
            int reminder = actualBits % requiredBits ? actualBits % requiredBits : requiredBits;
            result[i] = extract_bits(words, j, j + reminder);

        } else {
            result[i] = extract_bits(words, j, j + requiredBits - 1);
            j += requiredBits;
        }
    }
    return result;
}


std::vector<uint64_t> CellularAutomataPA::bitwise_and(std::vector<uint64_t> &vec1, std::vector<uint64_t> &vec2) {
    std::vector<uint64_t> result;
    result.reserve(vec1.size());
    for (int i = 0; i < vec1.size(); i++) {
        result.push_back(vec1[i] & vec2[i]);
    }
    return result;
}

std::vector<uint64_t> CellularAutomataPA::bitwise_xor(std::vector<uint64_t> &vec1, std::vector<uint64_t> &vec2) {
    std::vector<uint64_t> result;
    result.reserve(vec1.size());
    for (int i = 0; i < vec1.size(); i++) {
        result.push_back(vec1[i] ^ vec2[i]);
    }
    return result;
}


Key CellularAutomataPA::Amplificate(Key key, int K, int M, int N, std::vector<uint64_t> &initial_value) {
    int n_words=N/64+(N%64?1:0);
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
    std::vector<uint64_t> accumulator(N / 64 + (N % 64 ? 1 : 0), 0);

    // Step 3: Compress each group negotiation key
    std::vector<uint64_t> final_key(K * n_words, 0);
    for (int i = 0; i < K; i++) {
        std::vector<uint64_t> T_group = T_groups[i];
        std::vector<std::vector<uint64_t>> T_blocks = splitVector(T_group, N, M);
        for (auto T_block: T_blocks) {
            std::vector<uint64_t> ands = bitwise_and(T_block, CA);
            CA = update(CA, N);
            accumulator = bitwise_xor(accumulator, ands);
        }
        std::copy(accumulator.begin(), accumulator.end(), final_key.begin() + (i * n_words));
        std::copy(accumulator.begin(), accumulator.end(), CA.begin());
        CA = update(CA, N);
    }

    // Step 4: Combine the final key blocks into a final security key
    class Key PAKey(K*N, (((K*N)/64)+(((K*N)%64)? 1:0)) , combine_bits(&final_key, N));

    return PAKey;
}

//this function take a vector of words and combines them into full words
std::vector<uint64_t> CellularAutomataPA::combine_bits(std::vector<uint64_t> *data, int numberOfBits) {
    int n_words= numberOfBits/64+(numberOfBits%64?1:0);
    int totalBits = (numberOfBits / n_words)* data->size();
    int k = 0;
    std::vector<uint64_t> result((totalBits / 64) + (totalBits % 64 ? 1 : 0), 0);
    int currentWordBits = 0;
    int actualbits;
    int cnt=0;
    while (totalBits > 0) {
        for (int i = n_words*cnt; i < (n_words*(1+cnt)) ; ++i) {
            if(i==(n_words*(1+cnt))-1)
                actualbits=numberOfBits%64;
            else
             actualbits = 64;
            if (currentWordBits > 0 && currentWordBits < 64) {
                int transferInLast = std::min(64 - currentWordBits, actualbits);
                // Mask for the lower N bits
                uint64_t lowerMask = (1ULL << transferInLast) - 1;
                uint64_t toTransfer = (((*data)[i] & lowerMask) << (currentWordBits));
                result[k] |= toTransfer;
                actualbits -= transferInLast;
                totalBits -= transferInLast;
                currentWordBits += transferInLast;
            }
            if (currentWordBits == 64) {
                k++;
                currentWordBits = 0;
            }
            if (currentWordBits == 0 && actualbits > 0) {
                result[k]|=((*data)[i] & (end_word_mask(actualbits)));
                currentWordBits += actualbits;
                totalBits -= actualbits;
                actualbits = 0;
            }
        }
        cnt++;
    }
    return result;
}


std::vector<uint64_t>
CellularAutomataPA::extract_bits(const std::vector<uint64_t> &data, int start_index, int end_index) {
    int requested_bits = end_index - start_index + 1;
    int start_word_index = start_index / 64;
    int start_bit_offset = start_index % 64;
    int end_word_index = end_index / 64;
    int end_bit_offset = end_index % 64;
    int k = 0;
    std::vector<uint64_t> result;

    if (start_word_index == end_word_index) {
        uint64_t mask = (1ull << (end_bit_offset - start_bit_offset + 1)) - 1;
        uint64_t bits = (data[start_word_index] >> start_bit_offset) & mask;
        result.push_back(bits);
    } else {
        int currentWordBits = 0;
        int current_start = start_bit_offset;
        int current_end = std::min(start_bit_offset + requested_bits, 64);
        int i = start_index;
        while (requested_bits > 0) {
            int actual_bits = 64 - current_start;
            if (currentWordBits == 0) {
                result.push_back(data[i / 64] >> current_start & (end_word_mask(current_end - current_start)));
                currentWordBits += current_end - current_start;
                i += current_end - current_start;
                current_start = 0;
                actual_bits = 0;
                requested_bits -= currentWordBits;
                current_end = std::min(current_start + requested_bits, 64);
            } else {
                while (actual_bits > 0 && requested_bits > 0) {
                    if (currentWordBits == 64) {
                        k++;
                        currentWordBits = 0;
                        break;
                    }
                    int transferInLast = std::min(64 - currentWordBits, actual_bits);
                    transferInLast = std::min(transferInLast, requested_bits);
                    // Mask for the lower N bits
                    uint64_t lowerMask = (1ULL << transferInLast) - 1;
                    uint64_t toTransfer = ((data[i / 64] & lowerMask) << (currentWordBits));
                    result[k] |= toTransfer;
                    actual_bits -= transferInLast;
                    requested_bits -= transferInLast;
                    i += transferInLast;
                    current_start = 64 - currentWordBits;
                    currentWordBits += transferInLast;
                }
            }

        }
    }
    return result;
}
