#ifndef MOCK_CLASSICAL_SESSION_H
#define MOCK_CLASSICAL_SESSION_H

#include "shuffled_key.h"
#include "Server.h"
#include "pending_item.h"
#include "stats.h"
#include "classical_session.h"
#include <map>
namespace Cascade {



    class MockClassicalSession   : public ClassicalSession
            {
    public:
         explicit MockClassicalSession(Server server);

         ~MockClassicalSession() override;

         void start_iteration_with_shuffle_seed(int iteration_nr, uint32_t shuffle_seed) override;

         int channel_correct_parities(int iterationNr, int startBit, int endBit) override;

        void closeConnection() override;


    private:
                Server fakeServer;
    };

} /* namespace Cascade */

#endif /* ifndef MOCK_CLASSICAL_SESSION_H */
