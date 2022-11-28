//
// Created by marco on 25/11/22.
//

#ifndef MYCASCADE_CLIENT_H
#define MYCASCADE_CLIENT_H

#include <cstdint>
#include "classical_session.h"

namespace Cascade {

    class Client {


    public:
        Client();

    private:
        void start_iteration_with_shuffle_seed(int iteration_nr,
                                                              uint64_t shuffle_seed);
        ClassicalSession channel;

    };

} // Cascade

#endif //MYCASCADE_CLIENT_H
