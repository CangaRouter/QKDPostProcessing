//
// Created by marco on 27/06/23.
//

#ifndef PRIVACYAMPLIFICATIONCPP_CLASSICALSESSION_H
#define PRIVACYAMPLIFICATIONCPP_CLASSICALSESSION_H
#include <vector>
#include <string>

typedef struct {
    int K;
    int M;
    int N;
    std::vector<uint64_t> initial_value;
} CellularAutomataConfig;

class ClassicalSession {
public:
    virtual ~ClassicalSession() = 0;


    virtual CellularAutomataConfig NegotiateServer()=0;
    virtual void NegotiateClient(int K, int M, int N, std::vector<uint64_t> initial_value)=0;
    virtual void CloseClient()=0;
    virtual void CloseServer()=0;
};


#endif //PRIVACYAMPLIFICATIONCPP_CLASSICALSESSION_H
