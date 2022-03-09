//
// Created by peng on 2021/6/8.
//

#include "client/deliver_server_client.h"
#include <iostream>
#include <comm.pb.h>
#include "glog/logging.h"

#define CONFIG_REPLACEMENT_DICT "replacement_dict"
#define CONFIG_SERVERS "servers"
#define CONFIG_SERVERS_IP "ip"
#define CONFIG_SERVERS_UP_COMMAND "up_command"
#define CONFIG_SERVERS_DOWN_COMMAND "down_command"
#define CONFIG_SERVERS_UPDATE_COMMAND "update_command"
#define CONFIG_SERVERS_CUSTOM_COMMAND "custom_command"
#define CONFIG_SERVERS_EXTRA_REPLACEMENT_DICT "extra_replacement_dict"


DeliverServerClient::DeliverServerClient() : configNode(YAML::LoadFile("config.yaml")) {}

DeliverServerClient::~DeliverServerClient() = default;

std::string DeliverServerClient::getReplacementValueRecursively(const std::string &key) const {
    const auto &dict = configNode[CONFIG_REPLACEMENT_DICT];
    const auto &valueNode = dict[key];
    if (valueNode.IsDefined()) {   // begin with key
        return getReplacementValueRecursively(valueNode.as<std::string>());
    }
    return key;
}

void DeliverServerClient::run() {
    std::string command;
    do {
        std::cout << "'o': up the cluster,\n 'c': update config file,\n"
                     "'i' (re)install neu-chain to all other servers(do not run), path: ~/remote \n"
                     "'d': down the cluster,\n 'e' to emit custom command:" << std::endl;
        std::getline(std::cin, command);
        if (command.size() == 1) {  // char
            switch (command[0]) {
                case 'o': {
                    serverUp();
                    break;
                }
                case 'c': {
                    updateConfigFile();
                    break;
                }
                case 'd': {
                    serverDown();
                    break;
                }
                case 'e': {
                    customCommand();
                    break;
                }
                case 'i': {
                    installCommand();
                    break;
                }
                default: {
                    LOG(INFO) << "unknown op";
                }
            }
        } else {
            // not a char, but a command
            std::cout << "passing value: " << command << "to all servers";
            customCommand(command);
        }
    } while (!command.empty());
}

void DeliverServerClient::serverUp() {
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        const auto &ip = getNodeStringRecursively(server[CONFIG_SERVERS_IP]);
        const auto &command = getNodeStringRecursively(server[CONFIG_SERVERS_UP_COMMAND]);
        deliver = createDeliverServer(ip);
        deliver->emitCommand("up", {command});
        LOG(INFO) << ip << ": " << command << " with up command emitted.";
        deliverList.push_back(std::move(deliver));
    }
}

void DeliverServerClient::updateConfigFile() {
    // reload config file
    configNode = YAML::LoadFile("config.yaml");
    docker_config_message message;
    auto &replacement = *message.mutable_replacement();
    for (const auto &pair: configNode[CONFIG_REPLACEMENT_DICT]) {
        replacement[pair.first.as<std::string>()] = getNodeStringRecursively(pair.second);
    }   // get replacement dict
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        docker_config_message tmpMessage = message;
        auto &tmpReplacement = *tmpMessage.mutable_replacement();
        for (const auto &pair: server[CONFIG_SERVERS_EXTRA_REPLACEMENT_DICT]) {
            tmpReplacement[pair.first.as<std::string>()] = getNodeStringRecursively(pair.second);
        }   // get total replacement dict
        const auto &ip = getNodeStringRecursively(server[CONFIG_SERVERS_IP]);
        const auto &command = getNodeStringRecursively(server[CONFIG_SERVERS_UPDATE_COMMAND]);
        deliver = createDeliverServer(ip);
        deliver->emitCommand("update", {command, tmpMessage.SerializeAsString()});
        LOG(INFO) << ip << ": " << command << " with update config file command emitted.";
        deliverList.push_back(std::move(deliver));
    }
}

void DeliverServerClient::serverDown() {
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        const auto &ip = getNodeStringRecursively(server[CONFIG_SERVERS_IP]);
        const auto &command = getNodeStringRecursively(server[CONFIG_SERVERS_DOWN_COMMAND]);
        deliver = createDeliverServer(ip);
        deliver->emitCommand("down", {command});
        LOG(INFO) << ip << ": " << command << " with down command emitted.";
        deliverList.push_back(std::move(deliver));
    }
}

void DeliverServerClient::customCommand() {
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        const auto &ip = getNodeStringRecursively(server[CONFIG_SERVERS_IP]);
        const auto &command = getNodeStringRecursively(server[CONFIG_SERVERS_CUSTOM_COMMAND]);
        customCommand(ip, command);
    }
}

void DeliverServerClient::customCommand(const std::string& ip, const std::string &command) {
    deliver = createDeliverServer(ip);
    deliver->emitCommand("custom", {command});
    LOG(INFO) << ip << ": " << command << " emitted.";
    deliverList.push_back(std::move(deliver));

}

void DeliverServerClient::customCommand(const std::string &command) {
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        const auto &ip = getNodeStringRecursively(server[CONFIG_SERVERS_IP]);
        customCommand(ip, command);
    }
}

std::vector<std::unique_ptr<Executor>> pendingExecution;

void DeliverServerClient::installCommand() {
    LOG(INFO) << "install neu-chain to all servers(also this server)";
    std::make_unique<Executor>("rm -rf ~/.ssh/known_hosts; \n");
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        const auto &ip = getNodeStringRecursively(server[CONFIG_SERVERS_IP]);
        std::ostringstream ss;
        ss << "ssh-keyscan -H " << ip << " >> ~/.ssh/known_hosts; \n"; // add remote to host
        ss << "sshpass -p 'neu1234.' ssh root@" << ip << " \"rm -rf ~/remote; mkdir ~/remote;\"; \n";
        std::make_unique<Executor>(ss.str());
    }
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        const auto &ip = getNodeStringRecursively(server[CONFIG_SERVERS_IP]);
        LOG(INFO) << "Copying files to server: " << ip;
        std::ostringstream ss;
        ss << "sshpass -p 'neu1234.' scp -r ~/release_0.27 root@" << ip << ":~/remote; \n"; // cp new files
        std::make_unique<Executor>(ss.str());
    }
//    for (const auto &server: configNode[CONFIG_SERVERS]) {
//        const auto &ip = getNodeStringRecursively(server[CONFIG_SERVERS_IP]);
//        LOG(INFO) << "Starting server: " << ip;
//        std::ostringstream ss;
//        ss << "sshpass -p 'neu1234.' ssh root@" << ip << " \"cd ~/remote/release_0.27; ./install.sh;\"" << '\n';
//        pendingExecution.emplace_back(std::make_unique<Executor>(ss.str()));
//    }
}
