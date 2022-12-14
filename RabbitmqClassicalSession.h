//
// Created by Marco Pappalardo on 21/07/2022.
//

#ifndef MYCASCADE_RABBITMQCLASSICALSESSION_H
#define MYCASCADE_RABBITMQCLASSICALSESSION_H
#include "shuffled_key.h"
#include "Server.h"
#include "pending_item.h"
#include "stats.h"
#include "classical_session.h"
#include <map>
namespace Cascade {



    class RabbitmqClassicalSession   : public ClassicalSession
            {
            public:
                explicit RabbitmqClassicalSession(Server server);

        RabbitmqClassicalSession(std::string queue, std::string exchange, std::string host,
                                 std::string port);

        ~RabbitmqClassicalSession() override;

                void start_iteration_with_shuffle_seed(int iteration_nr, uint32_t shuffle_seed) override;

                int channel_correct_parities(int iterationNr, int startBit, int endBit) override;


                void test (int deltas);

            private:
                const std::string queue;
                const std::string exchange;
                const std::string host;
                const std::string port;
            };

} /* namespace Cascade */

#endif /* ifndef MOCK_CLASSICAL_SESSION_H */
