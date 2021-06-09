//
// Created by peng on 2021/6/8.
//

#include "server/executor.h"
#include "glog/logging.h"
#include <cstdio>

Executor::Executor(std::string _command) {
    std::string command(std::move(_command));
    pp = popen(command.data(), "r"); // build pipe
    if (!pp) {
        DLOG(ERROR) << "open pipe failed.";
        return;
    }
}

Executor::~Executor() {
    pclose(static_cast<FILE*>(pp));
}

void Executor::join(bool print) {
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), static_cast<FILE*>(pp)) != nullptr) {
        if(print)
            LOG(INFO) << buffer; // wait until command is finished
    }
}
