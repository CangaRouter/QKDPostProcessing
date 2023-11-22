//
// Created by marco on 25/11/22.
//

#include <sstream>
#include "Server.h"


namespace Cascade {


    Server::Server(Key &correctKey, bool cacheShuffles) : correct_key(correctKey), cache_shuffles(cacheShuffles) {


    }


    Server::Server(Key &correctKey, bool cacheShuffles, const std::string &host, int port, const std::string &user,
                   const std::string &pw, double noise, const std::string seq) : correct_key(correctKey),
                                                                                 cache_shuffles(cacheShuffles),
                                                                                 host(host),
                                                                                 port(port), user(user), pw(pw),
                                                                                 noise(noise), id(seq) {

        serverLoop();

    }

    Server::~Server() = default;

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

    // this is the main loop that the server executes, as it is basically listening to the classical channel to response
    void Server::serverLoop() {
        ConnHandler handler;
        AMQP::TcpConnection connection(handler,
                                       AMQP::Address(host, port,
                                                     AMQP::Login(user, pw), "/"));
        AMQP::TcpChannel channel(&connection);
        channel.onError([&handler](const char *message) {
            std::cout << "Channel error: " << message << std::endl;
            handler.Stop();
        });
        channel.setQos(1);
        channel.declareQueue("serverQueue" + id);
        channel.onError(
                [&, this](const char *message) {
                    std::cout << "Channel error: " << message << std::endl;
                    handler.Stop();
                }
        );

        channel.consume("serverQueue" + id, tag + "_Server",AMQP::noack+ AMQP::exclusive)
                .onReceived
                        (
                                [&, this](const AMQP::Message &message, uint64_t tag, bool) {
                                    AMQP::Table receivedHeaders = message.headers();
                                    std::string messageType = receivedHeaders.get("messageType");
                                    if (strcmp(messageType.c_str(), "startIteration") == 0) {
                                        start_iteration_with_shuffle_seed(receivedHeaders.get("nIteration"),
                                                                          receivedHeaders.get("seed"));
                                    } else if (strcmp(messageType.c_str(), "cascade_rpc") == 0) {
                                        rpcRound(&channel, message, receivedHeaders);
                                    } else if (strcmp(messageType.c_str(), "initialization") == 0) {
                                        initialization(&channel, message);
                                    } else if (strcmp(messageType.c_str(), "testConnection") == 0);
                                    else if (strcmp(messageType.c_str(), "closing") == 0) {
                                        closingConnection(&handler, &channel, message, &connection);
                                    } else {
                                        std::cout << "unrecognized message type, with tag: " << tag << std::endl;
                                    }
                                }
                        );
        handler.Start();

    }

    void Server::rpcRound(AMQP::TcpChannel *channel, const AMQP::Message &message,
                          const AMQP::Table &receivedHeaders) {
        int nBlocks = receivedHeaders.get("numberOfBlocks");
        std::string parities = "";
        std::vector<std::string> lines;
        std::vector<std::string> fields;
        boost::split(lines, message.body(), boost::is_any_of("\n"));
        for (int i = 0; i < nBlocks; i++) {
            boost::split(fields, lines[i], boost::is_any_of(","));
            parities += std::to_string(
                    return_correct_parities(std::stoi(fields[0]), std::stoi(fields[1]),
                                            std::stoi(fields[2]))) + ",";
        }
        AMQP::Envelope env(parities.c_str(), parities.size());
        AMQP::Table headers;
        headers.set("numberOfBlocks", nBlocks);
        headers.set("messageType", "Reply_rpc");
        env.setHeaders(headers);
        env.setCorrelationID(message.correlationID());
        channel->publish("", message.replyTo(), env);
    }

    const std::string &Server::getTag() const {
        return tag;
    }

    void Server::setTag(const std::string &tag) {
        Server::tag = tag;
    }

    void
    Server::closingConnection(ConnHandler *handler, AMQP::TcpChannel *channel, const AMQP::Message &message,
                              AMQP::TcpConnection *connection) const {
        std::string correctWords;
        for (auto &it: correct_key.getWords()) {
            correctWords += std::to_string(it) + '\n';
        }
        AMQP::Envelope env(correctWords.c_str());
        AMQP::Table headers;
        headers.set("nrBits", correct_key.get_nr_bits());
        headers.set("nrWords", correct_key.getNrWords());
        headers.set("messageType", "closingConfirm");
        env.setHeaders(headers);
        env.setCorrelationID(message.correlationID());
        channel->publish("", message.replyTo(), env);
        channel->cancel(this->tag + "_Server");
        channel->close();
        connection->close();
    }

    void Server::initialization(AMQP::TcpChannel *channel, const AMQP::Message &message) const {
        std::string noisyWords;
        Key noisyKey(correct_key);
        noisyKey.apply_noise(noise);
        for (auto &it: noisyKey.getWords()) {
            noisyWords += std::to_string(it) + '\n';
        }
        AMQP::Envelope envelope(noisyWords.c_str());
        AMQP::Table headers;
        headers.set("messageType", "initializationResponse");
        headers.set("nrBits", noisyKey.get_nr_bits());
        headers.set("nrWords", noisyKey.getNrWords());
        envelope.setHeaders(headers);
        envelope.setCorrelationID(message.correlationID());
        channel->publish("", message.replyTo(), envelope);
    }


    const Key &Server::getCorrectKey() const {
        return correct_key;
    }

    const std::string &Server::getHost() const {
        return host;
    }

    int Server::getPort() const {
        return port;
    }

    const std::string &Server::getUser() const {
        return user;
    }

    const std::string &Server::getPw() const {
        return pw;
    }

    double Server::getNoise() const {
        return noise;
    }


} // Cascade