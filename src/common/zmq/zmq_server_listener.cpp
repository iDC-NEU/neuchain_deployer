//
// Created by peng on 2021/4/23.
//

#include "common/zmq/zmq_server_listener.h"
#include <string>

ServerListener::ServerListener(std::string _port)
        : port(std::move(_port)), localIP("127.0.0.1") {}

ServerListener::~ServerListener() {
    // must wait until all client destroyed
    for (auto *client: remoteReceiverThreads) {
        client->join();
        delete client;
    }
    for (auto *client: clients) {
        delete client;
    }
}
