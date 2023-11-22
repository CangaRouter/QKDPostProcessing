//
// Created by marco on 31/12/22.
//

#ifndef MYCASCADE_CONN_HANDLER_H
#define MYCASCADE_CONN_HANDLER_H

#include <functional>
#include <unistd.h>
#include <event2/event.h>
#include <amqpcpp/libevent.h>
#include <iostream>
#include <memory>
class MyLibEventHandler : public AMQP::LibEventHandler {
public:
    MyLibEventHandler(struct event_base *evbase) : LibEventHandler(evbase){}
    uint16_t onNegotiate(AMQP::TcpConnection *connection, uint16_t interval) override {
        //we don't want to use heartbeats, so we return 0
        if (interval != 0) interval = 0;
        return interval;
    }
};
class ConnHandler {
public:
    ConnHandler() {
        evbase_ = event_base_new_with_config(cfg);
        evhandler_ = new MyLibEventHandler(evbase_);
    }
    ~ConnHandler() {
        event_base_free(evbase_);
        delete evhandler_;
    }

    void Start() {
        event_base_dispatch(evbase_);
    }

    void Stop() {
        event_base_loopbreak(evbase_);
    }

    operator AMQP::TcpHandler* () {
        return evhandler_;
    }

private:
    struct event_config *cfg = event_config_new();
    event_base* evbase_;
    MyLibEventHandler *evhandler_;
};


#endif //MYCASCADE_CONN_HANDLER_H
