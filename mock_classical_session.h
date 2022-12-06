#ifndef MOCK_CLASSICAL_SESSION_H
#define MOCK_CLASSICAL_SESSION_H

#include "shuffled_key.h"
#include "Server.h"
#include "pending_item.h"
#include "stats.h"
#include <map>
namespace Cascade {



    class MockClassicalSession   //: public ClassicalSession  *uncomment to use mock
            {
    public:
        MockClassicalSession(Key &correct_key, bool cache_shuffles, Server server);

        virtual ~MockClassicalSession();

        virtual void start_iteration_with_shuffle_seed(int iteration_nr, uint32_t shuffle_seed);


        int channel_correct_parities(int iterationNr, int startBit, int endBit);


        void test (int deltas);

    private:
        Key &correct_key;
        bool cache_shuffles;
        std::map<int, ShuffledKeyPtr> shuffled_keys;
        Server server;
        PendingItemQueue pending_ask_correct_parity_blocks;
        PendingItemQueue pending_try_correct_blocks;
        Stats stats;


    };

} /* namespace Cascade */

#endif /* ifndef MOCK_CLASSICAL_SESSION_H */
