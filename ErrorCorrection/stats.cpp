#include "stats.h"

using namespace Cascade;

Stats::Stats():
        elapsed_process_time(0.0),
        elapsed_real_time(0.0),
        normal_iterations(0),
        biconf_iterations(0),
        start_iteration_messages(0),
        start_iteration_bits(0),
        ask_parity_messages(0),
        ask_parity_blocks(0),
        ask_parity_bits(0),
        reply_parity_bits(0),
        unrealistic_efficiency(0.0),
        realistic_efficiency(0.0),
        infer_parity_blocks(0),
        keySize(0),
        correctKey(0)
{
}

int Stats::getActualBitErrors() const {
    return actual_bit_errors;
}

void Stats::setActualBitErrors(int actualBitErrors) {
    actual_bit_errors = actualBitErrors;
    actual_bit_error_rate=double(actualBitErrors)/double(keySize);
}

int Stats::getRemainingBitErrors() const {
    return remaining_bit_errors;
}

void Stats::setRemainingBitErrors(int remainingBitErrors) {
    remaining_bit_errors = remainingBitErrors;
    remaining_bit_error_rate=double(remainingBitErrors)/double(keySize);
}

double Stats::getActualBitErrorRate() const {
    return actual_bit_error_rate;
}

double Stats::getRemainingBitErrorRate() const {
    return remaining_bit_error_rate;
}

const Key &Stats::getCorrectKey() const {
    return correctKey;
}

void Stats::setCorrectKey(const Key &correctKey) {
    Stats::correctKey = correctKey;
}
