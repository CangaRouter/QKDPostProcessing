#ifndef STATS_H
#define STATS_H

#include "key.h"

namespace Cascade {

    class Stats
    {
    public:
        Stats();

        int getActualBitErrors() const;

        void setActualBitErrors(int actualBitErrors);

        int getRemainingBitErrors() const;

        void setRemainingBitErrors(int remainingBitErrors);

        double getActualBitErrorRate() const;

        double getRemainingBitErrorRate() const;

        int keySize;
        double elapsed_process_time;
        double elapsed_real_time;
        long normal_iterations;
        long biconf_iterations;
        long start_iteration_messages;
        long start_iteration_bits;
        long ask_parity_messages;
        long ask_parity_blocks;
        long ask_parity_bits;
        long reply_parity_bits;
        double unrealistic_efficiency;
        double realistic_efficiency;
        long infer_parity_blocks; // Only simple inference; not yet Full Block Parity Inference (BPI)
    private:
        int actual_bit_errors;
        double actual_bit_error_rate;
        int remaining_bit_errors;
        double remaining_bit_error_rate;
        double remaining_frame_error_rate;
        Key correctKey;
    public:
        const Key &getCorrectKey() const;

        void setCorrectKey(const Key &correctKey);

    };


} /* namespace Cascade */

#endif /* ifndef STATS_H */
