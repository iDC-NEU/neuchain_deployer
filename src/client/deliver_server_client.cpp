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
#define CONFIG_SERVERS_EXTRA_REPLACEMENT_DICT "extra_replacement_dict"

DeliverServerClient::DeliverServerClient() : configNode(YAML::LoadFile("config.yaml")) {}

DeliverServerClient::~DeliverServerClient() = default;

std::string DeliverServerClient::getReplacementValueRecursively(const std::string &key) const {
    const auto &dict = configNode[CONFIG_REPLACEMENT_DICT];
    const auto &value = dict[key].as<std::string>();
    if (!dict[value].IsNull()) {   // begin with key
        return getReplacementValueRecursively(value);
    }   // return value
    return value;
}

void DeliverServerClient::run() {
    std::string command;
    do {
        std::cout << "'o': up the cluster, 'c': update config file, "
                     "'d': down the cluster, 'e' to exit:" << std::endl;
        std::cin >> command;
        if (command == "o") {
            serverUp();
        } else if (command == "c") {
            updateConfigFile();
        } else if (command == "d") {
            serverDown();
        }
    } while (!command.empty());
}

void DeliverServerClient::serverUp() {
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        const auto &ip = getReplacementValueRecursively(server[CONFIG_SERVERS_IP].as<std::string>());
        const auto &command = getReplacementValueRecursively(server[CONFIG_SERVERS_UP_COMMAND].as<std::string>());
        deliver = createDeliverServer(ip);
        deliver->emitCommand("up", {command});
        LOG(INFO) << ip << ": " << command << " with up command emitted.";
        deliverList.push_back(std::move(deliver));
    }
}

void DeliverServerClient::updateConfigFile() {
    docker_config_message message;
    auto &replacement = *message.mutable_replacement();
    for (const auto &pair: configNode[CONFIG_REPLACEMENT_DICT]) {
        replacement[pair.first.as<std::string>()] = getReplacementValueRecursively(pair.second.as<std::string>());
    }   // get replacement dict
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        docker_config_message tmpMessage = message;
        auto &tmpReplacement = *tmpMessage.mutable_replacement();
        for (const auto &pair: server[CONFIG_SERVERS_EXTRA_REPLACEMENT_DICT]) {
            tmpReplacement[pair.first.as<std::string>()] = getReplacementValueRecursively(
                    pair.second.as<std::string>());
        }   // get total replacement dict
        const auto &ip = getReplacementValueRecursively(server[CONFIG_SERVERS_IP].as<std::string>());
        const auto &command = getReplacementValueRecursively(server[CONFIG_SERVERS_DOWN_COMMAND].as<std::string>());
        deliver = createDeliverServer(ip);
        deliver->emitCommand("update", {command, tmpMessage.SerializeAsString()});
        LOG(INFO) << ip << ": " << command << " with update config file command emitted.";
        deliverList.push_back(std::move(deliver));
    }
}

void DeliverServerClient::serverDown() {
    for (const auto &server: configNode[CONFIG_SERVERS]) {
        const auto &ip = getReplacementValueRecursively(server[CONFIG_SERVERS_IP].as<std::string>());
        const auto &command = getReplacementValueRecursively(server[CONFIG_SERVERS_UPDATE_COMMAND].as<std::string>());
        deliver = createDeliverServer(ip);
        deliver->emitCommand("down", {command});
        LOG(INFO) << ip << ": " << command << " with down command emitted.";
        deliverList.push_back(std::move(deliver));
    }
}
