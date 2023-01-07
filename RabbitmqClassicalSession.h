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
#include <amqpcpp.h>
#include "conn_handler.h"
namespace Cascade {



    class RabbitmqClassicalSession   : public ClassicalSession
            {
            public:

        RabbitmqClassicalSession(std::string host, int port, std::string user, std::string pw);

        ~RabbitmqClassicalSession() override;

                void start_iteration_with_shuffle_seed(int iteration_nr, uint32_t shuffle_seed) override;

                int channel_correct_parities(int iterationNr, int startBit, int endBit) override;

        void closeConnection() override;


    private:
        std::string host;
        int port;
        std::string user;
        std::string pw;
        AMQP::TcpConnection* connection;
        ConnHandler handler;
        AMQP::TcpChannel* channel;

    };

} /* namespace Cascade */

#endif /* ifndef MOCK_CLASSICAL_SESSION_H */
