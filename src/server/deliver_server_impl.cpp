//
// Created by peng on 2021/6/8.
//

#include "server/deliver_server_impl.h"
#include <fstream>
#include <glog/logging.h>
#include <comm.pb.h>

DeliverServerImpl::~DeliverServerImpl() {
    for (auto &command: pendingExecution) {
        command->join();
    }
}

void DeliverServerImpl::upCommand(const std::string &command) {
    if (command.empty()) {
        LOG(WARNING) << "folder name is empty";
    }
    // command represent the dir of docker compose file
    pendingExecution.emplace_back(std::make_unique<Executor>(
            "cd " + command + ";\ndocker-compose -f docker-compose-modify.yaml up --force-recreate"));
}

void DeliverServerImpl::downCommand(const std::string &command) {
    if (command.empty()) {
        LOG(WARNING) << "folder name is empty";
    }
    // command represent the dir of docker compose file
    pendingExecution.emplace_back(std::make_unique<Executor>(
            "cd " + command +
            ";\ndocker-compose -f docker-compose-modify.yaml down;\n"
            "(test -z \\\"$(docker ps -aq)\\\") || docker rm $(docker ps -aq);\n"
            "(test -z \\\"$(docker images dev* -q)\\\") || docker rmi $(docker images dev* -q);\n"
            "rm -rf /tmp/hfc-*;\n"
            "docker volume rm $(docker volume ls -qf dangling=true)"));
    pendingExecution.back()->join();
}

void DeliverServerImpl::updateCommand(const std::string &command, const std::string &messageRaw) {
    // command represent the folder name;
    const auto &updateFile = [](const std::string &oldFilePath, const std::string &newFilePath,
                                const std::string &messageRaw) {
        // the oldFilePath param is the docker compose file name;
        docker_config_message message;
        message.ParseFromString(messageRaw);
        const auto &dict = message.replacement();
        std::string fileData;
        if (!loadConfigFile(fileData, oldFilePath)) {
            LOG(WARNING) << "failed loading file: " << oldFilePath;
            return;
        }
        // 2. replace str
        IDockerComposeDeliverServer::replaceConfigFileWithDict(fileData, dict);
        // 3. save it
        std::ofstream file(newFilePath, std::ios::out | std::ios::binary);
        file.write(fileData.data(), fileData.size());
        file.close();
        LOG(WARNING) << "file updated: " << newFilePath;
    };
    updateFile("./caliper_config/network.json",
               "./caliper_config/network_modify.json",
               messageRaw);
    if (command.empty()) {
        LOG(WARNING) << "folder name is empty, docker compose will not update";
        return;
    }
    updateFile("./" + command + "/docker-compose.yaml",
               "./" + command + "/docker-compose-modify.yaml",
               messageRaw);
}

void DeliverServerImpl::emitCommand(const std::string &type, std::initializer_list<std::string> command) {
    if (type == "custom") {
        if (!command.size() || command.begin()->empty()) {
            LOG(WARNING) << "custom command is empty";
            return;
        }
        LOG(INFO) << "exec custom command: " << *command.begin();
        pendingExecution.emplace_back(std::make_unique<Executor>(*command.begin()));
    }
}
