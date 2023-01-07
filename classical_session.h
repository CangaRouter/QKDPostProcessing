#ifndef CLASSICAL_SESSION_H
#define CLASSICAL_SESSION_H

#include "block.h"
#include "pending_item.h"
#include "shuffle.h"
#include <stdint.h>


namespace Cascade {
    class ClassicalSession
            {
            public:
                virtual ~ClassicalSession() = 0;
                virtual void start_iteration_with_shuffle_seed(int , uint32_t ) =0;
                virtual int channel_correct_parities(int , int , int ) =0 ;
                virtual void closeConnection()=0;
    };

} /* namespace Cascade */

#endif /* ifndef CLASSICAL_SESSION_H */