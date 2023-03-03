cmake_minimum_required(VERSION 3.19)
project(Mycascade)

set(CMAKE_CXX_STANDARD 17)

add_executable(Mycascade main.cpp Client.cpp Client.h block.cpp block.h algorithm.cpp algorithm.h classical_session.cpp classical_session.h debug.cpp debug.h iteration.cpp iteration.h key.cpp key.h mock_classical_session.cpp mock_classical_session.h pending_item.cpp pending_item.h random.cpp random.h report.cpp report.h shuffle.cpp shuffle.h shuffled_key.cpp shuffled_key.h stats.cpp stats.h RabbitmqClassicalSession.cpp RabbitmqClassicalSession.h Server.cpp Server.h conn_handler.h)
target_link_libraries(Mycascade amqpcpp boost_system boost_thread  boost_chrono pthread event ssl crypto)
