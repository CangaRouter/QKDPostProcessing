//
// Created by marco on 01/07/23.
//

#ifndef PRIVACYAMPLIFICATIONCPP_RANDOM_H
#define PRIVACYAMPLIFICATIONCPP_RANDOM_H
#include <random>
#include <stdint.h>

void set_random_uint32_seed(uint32_t seed);
uint32_t random_uint32();
int random_bit_nr(int start_bit_nr, int end_bit_nr);


#endif //PRIVACYAMPLIFICATIONCPP_RANDOM_H
