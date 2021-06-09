//
// Created by peng on 2021/6/8.
//

#include "comm.pb.h"
#include "server/deliver_server_controller_impl.h"
#include "common/zmq/zmq_server.h"
#include "glog/logging.h"

void DeliverServerControllerImpl::configDockerContainer(const std::string &commandRaw, const std::string& folder) {
    docker_config_message message;
    CHECK(message.ParseFromString(commandRaw));
    // 1. load template
    if(!message.raw_docker_compose_data().empty()) {
        deliver->setDockerComposeFile(message.raw_docker_compose_data());
    } else {
        deliver->setDockerComposeFile(folder, "docker-compose.yaml");
    }
    // 2. replace str
    std::map<std::string, std::string> replacement;
    for (const auto& pair: message.replacement()) {
        replacement[pair.first] = pair.second;
    }
    deliver->strReplace(replacement);
    // 3. save it
    deliver->saveDockerComposeFile(folder);
}

void DeliverServerControllerImpl::startDockerContainer(const std::string& folder) {
    deliver->upDockerCompose(folder);
}

void DeliverServerControllerImpl::stopDockerContainer(const std::string& folder) {
    deliver->downDockerCompose(folder);
}

void DeliverServerControllerImpl::run() {
    ZMQServer server("0.0.0.0", "8888");
    auto deserializeRequest = [&](const std::string& requestRaw) -> docker_control_message {
        docker_control_message message;
        message.ParseFromString(requestRaw);
        return std::move(message);
    };
    while(true) {
        std::string requestRaw;
        server.getRequest(requestRaw);
        auto message = deserializeRequest(requestRaw);
        if(message.status() == docker_control_message_Status_EXIT) {
            break;
        }
        // process request
        switch (message.status()) {
            case docker_control_message_Status_UP: {
                startDockerContainer(message.folder());
                break;
            }
            case docker_control_message_Status_DOWN: {
                stopDockerContainer(message.folder());
                break;
            }
            case docker_control_message_Status_CONFIG: {
                configDockerContainer(message.additional_data(), message.folder());
                break;
            }
            default:
                LOG(ERROR) << "wrong control message.";
                CHECK(false);
        }
        server.sendReply();
    }
}

DeliverServerControllerImpl::~DeliverServerControllerImpl() = default;
