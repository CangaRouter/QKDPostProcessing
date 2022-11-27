//
// Created by marco on 25/11/22.
//

#ifndef MYCASCADE_RECEIVER_H
#define MYCASCADE_RECEIVER_H

#include <cstdint>
#include "classical_session.h"

namespace Cascade {

    class Receiver {


    public:
        Receiver();

    private:
        void start_iteration_with_shuffle_seed(int iteration_nr,
                                                              uint64_t shuffle_seed);
        ClassicalSession channel;

    };

} // Cascade

#endif //MYCASCADE_RECEIVER_H
