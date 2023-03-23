#include "data_point.h"
#include "stats.h"
#include <iomanip>
#include <sstream>

DataPoint::DataPoint(const std::string &algorithm_name, int key_size,
                     double requested_bit_error_rate) :
        algorithm_name(algorithm_name),
        key_size(key_size),
        requested_bit_error_rate(requested_bit_error_rate),
        reconciliations(0) {
}

void DataPoint::record_reconciliation_stats(const Cascade::Stats &stats) {
    reconciliations += 1;
    elapsed_process_time=stats.elapsed_process_time;
    elapsed_real_time=stats.elapsed_real_time;
    normal_iterations=stats.normal_iterations;
    biconf_iterations=stats.biconf_iterations;
    ask_parity_messages=stats.ask_parity_messages;
    ask_parity_blocks=stats.ask_parity_blocks;
    ask_parity_bits=stats.ask_parity_bits;
    reply_parity_bits=stats.reply_parity_bits;
    unrealistic_efficiency=stats.unrealistic_efficiency;
    realistic_efficiency=stats.realistic_efficiency;
    infer_parity_blocks=stats.infer_parity_blocks;
    actual_bit_errors=stats.getActualBitErrors();
    actual_bit_error_rate=stats.getActualBitErrorRate();
    remaining_bit_errors=stats.getRemainingBitErrors();
    remaining_bit_error_rate=stats.getRemainingBitErrorRate();
    correctKey = stats.getCorrectKey().to_string();
}

std::string DataPoint::to_json() const {
    std::string json = "{";

    // Actual bit error rate
    json += "\"actual_bit_error_rate\": " + std::to_string(actual_bit_error_rate) + ", ";

    // Actual bit errors
    json += "\"actual_bit_errors\": " + std::to_string(actual_bit_errors) + ", ";

    // Algorithm name
    json += "\"algorithm_name\": \"" + algorithm_name + "\", ";

    // Ask parity bits
    json += "\"ask_parity_bits\": " + std::to_string(ask_parity_bits) + ", ";

    // Ask parity blocks
    json += "\"ask_parity_blocks\": " + std::to_string(ask_parity_blocks) + ", ";

    // Ask parity messages
    json += "\"ask_parity_messages\": " + std::to_string(ask_parity_messages) + ", ";

    // BICONF iterations
    json += "\"biconf_iterations\": " + std::to_string(biconf_iterations) + ", ";

    // Elapsed process time
    json += "\"elapsed_process_time\": " + std::to_string(elapsed_process_time) + ", ";

    // Elapsed real time
    json += "\"elapsed_real_time\": " + std::to_string(elapsed_real_time) + ", ";

    // Execution time: the date and time at which the data point was produced, i.e. now.
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream now_str;
    now_str << std::put_time(&tm, "%Y-%m-%d %H:%M:%S %Z");
    json += "\"execution_time\": \"" + now_str.str() + "\", ";

    // Infer parity blocks
    json += "\"infer_parity_blocks\": " + std::to_string(infer_parity_blocks) + ", ";

    // Key size
    json += "\"key_size\": " + std::to_string(key_size) + ", ";

    // Normal iterations
    json += "\"normal_iterations\": " + std::to_string(normal_iterations) + ", ";

    // Realistic efficiency
    json += "\"realistic_efficiency\": " + std::to_string(realistic_efficiency) + ", ";

    // Reconciliations
    json += "\"reconciliations\": " + std::to_string(reconciliations) + ", ";

    // Remaining bit error rate
    json += "\"remaining_bit_error_rate\": " + std::to_string(remaining_bit_error_rate) + ", ";

    // Remaining bit errors
    json += "\"remaining_bit_errors\": " + std::to_string(remaining_bit_errors) + ", ";

    // Remaining frame error rate
    json += "\"remaining_frame_error_rate\": " + std::to_string(remaining_frame_error_rate) + ", ";

    // Reply parity bits
    json += "\"reply_parity_bits\": " + std::to_string(reply_parity_bits) + ", ";

    // Requested bit error rate
    json += "\"requested_bit_error_rate\": " + std::to_string(requested_bit_error_rate) + ", ";

    // Unrealistic efficiency
    json += "\"unrealistic_efficiency\": " + std::to_string(unrealistic_efficiency) + ", ";

    json += R"("correctKey":  ")" + correctKey + "\"";
    json += "}";
    return json;
}

const std::string &DataPoint::getAlgorithmName() const {
    return algorithm_name;
}

void DataPoint::setAlgorithmName(const std::string &algorithmName) {
    algorithm_name = algorithmName;
}

int DataPoint::getKeySize() const {
    return key_size;
}

void DataPoint::setKeySize(int keySize) {
    key_size = keySize;
}

double DataPoint::getRequestedBitErrorRate() const {
    return requested_bit_error_rate;
}

void DataPoint::setRequestedBitErrorRate(double requestedBitErrorRate) {
    requested_bit_error_rate = requestedBitErrorRate;
}

long DataPoint::getReconciliations() const {
    return reconciliations;
}

void DataPoint::setReconciliations(long reconciliations) {
    DataPoint::reconciliations = reconciliations;
}

int DataPoint::getActualBitErrors() const {
    return actual_bit_errors;
}

void DataPoint::setActualBitErrors(int actualBitErrors) {
    actual_bit_errors = actualBitErrors;
}

double DataPoint::getActualBitErrorRate() const {
    return actual_bit_error_rate;
}

void DataPoint::setActualBitErrorRate(double actualBitErrorRate) {
    actual_bit_error_rate = actualBitErrorRate;
}

double DataPoint::getElapsedProcessTime() const {
    return elapsed_process_time;
}

void DataPoint::setElapsedProcessTime(double elapsedProcessTime) {
    elapsed_process_time = elapsedProcessTime;
}

double DataPoint::getElapsedRealTime() const {
    return elapsed_real_time;
}

void DataPoint::setElapsedRealTime(double elapsedRealTime) {
    elapsed_real_time = elapsedRealTime;
}

int DataPoint::getNormalIterations() const {
    return normal_iterations;
}

void DataPoint::setNormalIterations(int normalIterations) {
    normal_iterations = normalIterations;
}

int DataPoint::getBiconfIterations() const {
    return biconf_iterations;
}

void DataPoint::setBiconfIterations(int biconfIterations) {
    biconf_iterations = biconfIterations;
}

int DataPoint::getAskParityMessages() const {
    return ask_parity_messages;
}

void DataPoint::setAskParityMessages(int askParityMessages) {
    ask_parity_messages = askParityMessages;
}

int DataPoint::getAskParityBlocks() const {
    return ask_parity_blocks;
}

void DataPoint::setAskParityBlocks(int askParityBlocks) {
    ask_parity_blocks = askParityBlocks;
}

int DataPoint::getAskParityBits() const {
    return ask_parity_bits;
}

void DataPoint::setAskParityBits(int askParityBits) {
    ask_parity_bits = askParityBits;
}

int DataPoint::getReplyParityBits() const {
    return reply_parity_bits;
}

void DataPoint::setReplyParityBits(int replyParityBits) {
    reply_parity_bits = replyParityBits;
}

double DataPoint::getUnrealisticEfficiency() const {
    return unrealistic_efficiency;
}

void DataPoint::setUnrealisticEfficiency(double unrealisticEfficiency) {
    unrealistic_efficiency = unrealisticEfficiency;
}

double DataPoint::getRealisticEfficiency() const {
    return realistic_efficiency;
}

void DataPoint::setRealisticEfficiency(double realisticEfficiency) {
    realistic_efficiency = realisticEfficiency;
}

int DataPoint::getInferParityBlocks() const {
    return infer_parity_blocks;
}

void DataPoint::setInferParityBlocks(int inferParityBlocks) {
    infer_parity_blocks = inferParityBlocks;
}

int DataPoint::getRemainingBitErrors() const {
    return remaining_bit_errors;
}

void DataPoint::setRemainingBitErrors(int remainingBitErrors) {
    remaining_bit_errors = remainingBitErrors;
}

double DataPoint::getRemainingBitErrorRate() const {
    return remaining_bit_error_rate;
}

void DataPoint::setRemainingBitErrorRate(double remainingBitErrorRate) {
    remaining_bit_error_rate = remainingBitErrorRate;
}

double DataPoint::getRemainingFrameErrorRate() const {
    return remaining_frame_error_rate;
}

void DataPoint::setRemainingFrameErrorRate(double remainingFrameErrorRate) {
    remaining_frame_error_rate = remainingFrameErrorRate;
}

const std::string &DataPoint::getCorrectKey() const {
    return correctKey;
}

void DataPoint::setCorrectKey(const std::string &correctKey) {
    DataPoint::correctKey = correctKey;
}
