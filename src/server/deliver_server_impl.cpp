//
// Created by peng on 2021/6/8.
//

#include "server/deliver_server_impl.h"
#include <algorithm>
#include <fstream>

void DeliverServerImpl::saveDockerComposeFile(const std::string &appName) {
    Executor executor("mkdir " + appName);
    executor.join();
    std::ofstream file(appName + "/" + "docker-compose-modify.yaml", std::ios::out | std::ios::binary);
    file.write(fileData.data(), fileData.size());
    file.close();
}

void DeliverServerImpl::upDockerCompose(const std::string &appName) {
    auto command = std::make_unique<Executor>(
            "cd " +
            appName +
            ";\ndocker-compose -f docker-compose-modify.yaml up --force-recreate");
    pendingExecution.push_back(std::move(command));
}

void DeliverServerImpl::downDockerCompose(const std::string &appName) {
    auto command = std::make_unique<Executor>(
            "cd " +
            appName +
            ";\ndocker-compose -f docker-compose-modify.yaml down;\n"
            "(test -z \\\"$(docker ps -aq)\\\") || docker rm $(docker ps -aq);"
            "(test -z \\\"$(docker images dev* -q)\\\") || docker rmi $(docker images dev* -q);"
            "rm -rf /tmp/hfc-*\n"
            "docker volume rm $(docker volume ls -qf dangling=true)");
    pendingExecution.push_back(std::move(command));
}

DeliverServerImpl::~DeliverServerImpl() {
    for(auto& command: pendingExecution) {
        command->join();
    }
}
