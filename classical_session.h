#ifndef CLASSICAL_SESSION_H
#define CLASSICAL_SESSION_H

#include "block.h"
#include "pending_item.h"
#include "shuffle.h"
#include "key.h"
#include <stdint.h>
#include "shuffled_key.h"
#include <map>
#include <AMQPcpp.h>

namespace Cascade {
  //remove complex constructor and add virtual to methods to use mock
    class ClassicalSession {
    public:
        ClassicalSession(Cascade::Key &correct_key, bool cache_shuffles);

        ClassicalSession(Key &correct_key, bool cache_shuffles, std::string hostip, std::string hostport, std::string exchange, std::string queue );

         ~ClassicalSession();

         void start_iteration_with_shuffle_seed(int iteration_nr, uint64_t shuffle_seed) ;

         void start_iteration_with_shuffle(int iteration_nr, ShufflePtr shuffle) ;

         void ask_correct_parities(PendingItemQueue &ask_correct_parity_blocks) ;

        int reply_correct_parities(int iteration_nr, int start_bit, int end_bit);

        void test (int deltas);

        void configureChannel(std ::string hostip ,std::string hostport, std::string exchange, std::string queue  );

        int parityRound(int startBit, int endBit, int iteration);

        void openSenderChannel();

        Key &getCorrectKey() const;

        void setCorrectKey(Key &correctKey);

        bool isCacheShuffles() const;

        void setCacheShuffles(bool cacheShuffles);

        const std::map<int, ShuffledKeyPtr> &getShuffledKeys() const;

        void setShuffledKeys(const std::map<int, ShuffledKeyPtr> &shuffledKeys);

        const std::string &getHostip() const;

        void setHostip(const std::string &hostip);

        const std::string &getHostport() const;

        void setHostport(const std::string &hostport);


    private:
        Key &correct_key;
        bool cache_shuffles;
        std::map<int, ShuffledKeyPtr> shuffled_keys;
        std ::string hostip;
        std::string hostport;
        std::string exchange;
        std::string queue;
        int seq=0;
        int senderbit= -1;
        int receiverbit=-1;
    };

} /* namespace Cascade */

#endif /* ifndef CLASSICAL_SESSION_H */
