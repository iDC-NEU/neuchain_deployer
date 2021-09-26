//
// Created by peng on 2021/6/9.
//

#include "server/aria_deliver_server_impl.h"
#include <fstream>
#include <glog/logging.h>
#include <comm.pb.h>

void AriaDeliverServerImpl::updateCommand(const std::string &command, const std::string &messageRaw) {
    // command represent command;
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
    updateFile("./bin/config-template.yaml", "./bin/config.yaml", messageRaw);
    if (command.empty()) {
        return;
    }
    // exec command after modify config
    pendingExecution.push_back(std::make_unique<Executor>(command));
    pendingExecution.back()->join();
}

void AriaDeliverServerImpl::upCommand(const std::string &command) {
    if (command.empty()) {
        return;
    }
    // command represent command;
    pendingExecution.push_back(std::make_unique<Executor>(command));
}

void AriaDeliverServerImpl::downCommand(const std::string &command) {
    // command represent command;
    pendingExecution.push_back(std::make_unique<Executor>(
            "kill -9 $(pidof user)\n"
            "kill -9 $(pidof block_server_test_comm)\n"
            "kill -9 $(pidof epoch_server)\n"
            "cd bin\n"
            "rm *.bin\n"
            "rm block_num.txt\n"
            "rm -rf small_bank\n"
            "rm -rf raft_data\n"
            "rm -rf test_table\n"
            + command));
    // append custom command
    pendingExecution.back()->join();
}

void AriaDeliverServerImpl::emitCommand(const std::string &type, std::initializer_list<std::string> command) {
    if (type == "custom") {
        if (!command.size() || command.begin()->empty()) {
            LOG(WARNING) << "custom command is empty";
            return;
        }
        LOG(INFO) << "exec custom command: " << *command.begin();
        pendingExecution.emplace_back(std::make_unique<Executor>(*command.begin()));
    }
}
