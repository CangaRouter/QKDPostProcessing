#ifndef DATA_POINT_H
#define DATA_POINT_H

#include <string>
namespace Cascade {
    class Stats;
}

class DataPoint
{
public:
    DataPoint(const std::string& algorithm_name, int key_size, double requested_bit_error_rate);
    void record_reconciliation_stats(const Cascade::Stats& stats);
    std::string to_json() const;

    const std::string &getAlgorithmName() const;

    void setAlgorithmName(const std::string &algorithmName);

    int getKeySize() const;

    void setKeySize(int keySize);

    double getRequestedBitErrorRate() const;

    void setRequestedBitErrorRate(double requestedBitErrorRate);

    long getReconciliations() const;

    void setReconciliations(long reconciliations);

    int getActualBitErrors() const;

    void setActualBitErrors(int actualBitErrors);

    double getActualBitErrorRate() const;

    void setActualBitErrorRate(double actualBitErrorRate);

    double getElapsedProcessTime() const;

    void setElapsedProcessTime(double elapsedProcessTime);

    double getElapsedRealTime() const;

    void setElapsedRealTime(double elapsedRealTime);

    int getNormalIterations() const;

    void setNormalIterations(int normalIterations);

    int getBiconfIterations() const;

    void setBiconfIterations(int biconfIterations);

    int getAskParityMessages() const;

    void setAskParityMessages(int askParityMessages);

    int getAskParityBlocks() const;

    void setAskParityBlocks(int askParityBlocks);

    int getAskParityBits() const;

    void setAskParityBits(int askParityBits);

    int getReplyParityBits() const;

    void setReplyParityBits(int replyParityBits);

    double getUnrealisticEfficiency() const;

    void setUnrealisticEfficiency(double unrealisticEfficiency);

    double getRealisticEfficiency() const;

    void setRealisticEfficiency(double realisticEfficiency);

    int getInferParityBlocks() const;

    void setInferParityBlocks(int inferParityBlocks);

    int getRemainingBitErrors() const;

    void setRemainingBitErrors(int remainingBitErrors);

    double getRemainingBitErrorRate() const;

    void setRemainingBitErrorRate(double remainingBitErrorRate);

    double getRemainingFrameErrorRate() const;

    void setRemainingFrameErrorRate(double remainingFrameErrorRate);

    const std::string &getCorrectKey() const;

    void setCorrectKey(const std::string &correctKey);

private:
    std::string algorithm_name;
    int key_size;
    double requested_bit_error_rate;
    long reconciliations;
    int actual_bit_errors;
    double actual_bit_error_rate;
    double elapsed_process_time;
    double elapsed_real_time;
    int normal_iterations;
    int biconf_iterations;
    int ask_parity_messages;
    int ask_parity_blocks;
    int ask_parity_bits;
    int reply_parity_bits;
    double unrealistic_efficiency;
    double realistic_efficiency;
    int infer_parity_blocks;
    int remaining_bit_errors;
    double remaining_bit_error_rate;
    double remaining_frame_error_rate;
    std::string correctKey;
};

#endif /* ifndef DATA_POINT_H */
