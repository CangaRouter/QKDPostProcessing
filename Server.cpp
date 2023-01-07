//
// Created by marco on 25/11/22.
//

#include <sstream>
#include "Server.h"


namespace Cascade {


    Server::Server(Key &correctKey, bool cacheShuffles) : correct_key(correctKey), cache_shuffles(cacheShuffles) {


    }


    Server::Server(Key &correctKey, bool cacheShuffles, const std::string &host, int port, const std::string &user,
                   const std::string &pw) : correct_key(correctKey), cache_shuffles(cacheShuffles), host(host),
                                            port(port), user(user), pw(pw) {

        serverLoop();

    }


    Server::~Server()=default;

    void Server::start_iteration_with_shuffle_seed(int iteration_nr,
                                                                 std::string shuffle_seed) {
        int nr_key_bits = correct_key.get_nr_bits();
        ShufflePtr shuffle;
        shuffle = Shuffle::new_shuffle_from_seed(iteration_nr, nr_key_bits, std::stoul(shuffle_seed),
                                                 cache_shuffles);
        ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
        shuffled_keys[iteration_nr] = shuffled_key;
    }


    int Server::return_correct_parities(int iterationNr, int startBit, int endBit) {
        // Once we implement the real classical session, we will need to keep track of the blocks
        // for which we asked Alice the correct parity, but for which we have not yet received the
        // answer from Alice. For now, assume we get the answer immediately.
            ShuffledKeyPtr shuffled_key = shuffled_keys[iterationNr];
            int correct_parity = shuffled_key->compute_range_parity(startBit,
                                                                    endBit);
            return correct_parity;
    }

    void Server::serverLoop(){
        ConnHandler handler;
        AMQP::TcpConnection connection(handler,
                                       AMQP::Address(host, port,
                                                     AMQP::Login(user, pw), "/"));
        AMQP::TcpChannel channel(&connection);
        channel.onError([&handler](const char* message)
                        {
                            std::cout << "Channel error: " << message << std::endl;
                            handler.Stop();
                        });
        channel.setQos(1);
        channel.declareQueue("serverQueue");
         channel.consume("serverQueue","alfredo",AMQP::noack+ AMQP::exclusive)
                .onReceived
                        (
                                [&channel, this](const AMQP::Message& m, uint64_t tag, bool)
                                {
                                    int startBit=-1;
                                    int endBit=-1;
                                    int nIteration=-1;
                                    AMQP::Table headers=m.headers();
                                    if(headers.contains("startIterationWithSeed")){
                                        std::string seed=headers.get("startIterationWithSeed");
                                        nIteration=headers.get("nIteration");
                                        start_iteration_with_shuffle_seed(nIteration, seed);
                                    }
                                    else if (headers.contains(("startBit"))){
                                        startBit=headers.get("startBit");
                                        endBit=headers.get("endBit");
                                        nIteration=headers.get("nIteration");
                                        int response=return_correct_parities(nIteration, startBit, endBit);
                                        AMQP::Envelope env("",0);
                                        AMQP::Table headers;
                                        headers.set("parity",response);
                                        env.setHeaders(headers);
                                        env.setCorrelationID(m.correlationID());
                                        channel.publish("", m.replyTo(), env);
                                    }

                                    }

                        ).onCancelled([&](const std::string &consumertag){
                            handler.Stop();

         });
        handler.Start();
        connection.close();
    }




} // Cascade