cmake_minimum_required(VERSION 3.25)
project(PrivacyAmplificationCPP)

set(CMAKE_CXX_STANDARD 23)

add_executable(PrivacyAmplificationCPP main.cpp Server.cpp Server.h Client.cpp Client.h ClassicalSession.cpp ClassicalSession.h RabbitmqClassicalSession.cpp RabbitmqClassicalSession.h ConnHandler.h Key.cpp Key.h Random.cpp Random.h CellularAutomataPA.cpp CellularAutomataPA.h)
target_link_libraries(PrivacyAmplificationCPP amqpcpp boost_system boost_thread  boost_chrono pthread event ssl crypto)