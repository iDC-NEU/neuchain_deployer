//
// Created by peng on 2021/6/8.
//

#include "client/deliver_server_client.h"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "glog/logging.h"
#include <ctime>

DeliverServerClient::~DeliverServerClient() = default;

void DeliverServerClient::run() {
    std::string command;
    do {
        std::cout << "'o': up the cluster, 'c': update config file, "
                     "'d': down the cluster, 'e' to exit:" << std::endl;
        std::cin >> command;
        if(command == "o") {
            serverUp();
        } else if (command == "c") {
            updateConfigFile();
        } else if (command == "d") {
            serverDown();
        }
    } while (!command.empty());
}

void DeliverServerClient::serverUp() {
    auto data = YAML::LoadFile("config.yaml");
    const auto& replacement = data["hosts"];
    for(const auto& server: data["servers"]) {
        deliver = createDeliverServer(server["ip"].as<std::string>());
        deliver->upDockerCompose(server["folder_name"].as<std::string>());
        sleep(2);
        LOG(INFO) << server["folder_name"].as<std::string>() << " in " << server["ip"].as<std::string>() <<" is up.";
        deliverList.push_back(std::move(deliver));
    }
}

void DeliverServerClient::updateConfigFile() {
    auto data = YAML::LoadFile("config.yaml");
    std::map<std::string, std::string> replacement;
    for (const auto& pair: data["hosts"]) {
        replacement[pair.first.as<std::string>()] = pair.second.as<std::string>();
    }
    for(const auto& server: data["servers"]) {
        auto uniqueReplacement = replacement;
        for (const auto& pair: server["hosts"]) {
            uniqueReplacement[pair.first.as<std::string>()] = pair.second.as<std::string>();
        }
        deliver = createDeliverServer(server["ip"].as<std::string>());
        deliver->strReplace(uniqueReplacement);
        deliver->saveDockerComposeFile(server["folder_name"].as<std::string>());
        LOG(INFO) << server["folder_name"].as<std::string>() << " in " << server["ip"].as<std::string>() <<" is updated.";
        deliverList.push_back(std::move(deliver));
    }
}

void DeliverServerClient::serverDown() {
    auto data = YAML::LoadFile("config.yaml");
    for(const auto& server: data["servers"]) {
        deliver = createDeliverServer(server["ip"].as<std::string>());
        deliver->downDockerCompose(server["folder_name"].as<std::string>());
        deliverList.push_back(std::move(deliver));
    }
}
