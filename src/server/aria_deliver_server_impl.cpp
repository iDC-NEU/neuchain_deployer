//
// Created by peng on 2021/6/9.
//

#include "server/aria_deliver_server_impl.h"
#include <algorithm>
#include <fstream>

void AriaDeliverServerImpl::saveDockerComposeFile(const std::string &, const std::string &) {
    std::ofstream file("bin/config.yaml", std::ios::out | std::ios::binary);
    file.write(fileData.data(), fileData.size());
    file.close();
}

// appName = exeFile + params
void AriaDeliverServerImpl::upDockerCompose(const std::string &appName) {
    if(appName == "install") {
        auto command = std::make_unique<Executor>("echo installing...\n"
        "cp so/lib* /usr/lib/\n"
        "chmod 755 bin/block_server_test_comm\n"
        "chmod 755 bin/epoch_server\n"
        "chmod 755 bin/user\n");
        command->join();
    } else {
        auto command = std::make_unique<Executor>("cd bin;\n" + appName);
        pendingExecution.push_back(std::move(command));
    }
}

void AriaDeliverServerImpl::downDockerCompose(const std::string &) {
    auto command = std::make_unique<Executor>("kill -9 $(pidof user)\n"
                                              "kill -9 $(pidof block_server_test_comm)\n"
                                              "kill -9 $(pidof epoch_server)\n"
                                              "cd bin\n"
                                              "rm *.bin\n"
                                              "rm block_num.txt\n"
                                              "rm -rf small_bank\n"
                                              "rm -rf test_table\n");
    command->join();
    pendingExecution.push_back(std::move(command));
}

void AriaDeliverServerImpl::setDockerComposeFile(const std::string &, const std::string&) {
    IDockerComposeDeliverServer::setDockerComposeFile("bin", "config-template.yaml");
}
