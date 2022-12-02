cmake_minimum_required(VERSION 3.19)
project(Mycascade)

set(CMAKE_CXX_STANDARD 14)

add_executable(Mycascade main.cpp Client.cpp Client.h block.cpp block.h algorithm.cpp algorithm.h classical_session.cpp classical_session.h debug.cpp debug.h iteration.cpp iteration.h key.cpp key.h mock_classical_session.cpp mock_classical_session.h pending_item.cpp pending_item.h random.cpp random.h report.cpp report.h shuffle.cpp shuffle.h shuffled_key.cpp shuffled_key.h stats.cpp stats.h RabbitmqClassicalSession.cpp RabbitmqClassicalSession.h Server.cpp Server.h)
target_link_libraries(Mycascade rabbitmq amqpcpp ssl crypto)
