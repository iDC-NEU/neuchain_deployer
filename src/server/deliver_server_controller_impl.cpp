//
// Created by peng on 2021/6/8.
//

#include "comm.pb.h"
#include "server/deliver_server_controller_impl.h"
#include "common/zmq/zmq_server.h"
#include "glog/logging.h"

void DeliverServerControllerImpl::run() {
    ZMQServer server("0.0.0.0", "8888");
    auto deserializeRequest = [&](const std::string &requestRaw) -> docker_control_message {
        docker_control_message message;
        CHECK(message.ParseFromString(requestRaw));
        return std::move(message);
    };
    while (true) {
        std::string requestRaw;
        server.getRequest(requestRaw);
        auto message = deserializeRequest(requestRaw);
        if (message.status() == docker_control_message_Status_EXIT) {
            break;
        }
        // process request
        switch (message.status()) {
            case docker_control_message_Status_UP: {
                deliver->upCommand(message.command());
                break;
            }
            case docker_control_message_Status_DOWN: {
                deliver->downCommand(message.command());
                break;
            }
            case docker_control_message_Status_CONFIG: {
                const std::string &command = message.command();
                const std::string &messageRaw = message.additional_data();
                deliver->updateCommand(command, messageRaw);
                break;
            }
            default:
                LOG(ERROR) << "unknown control message, pass to deliver server directly";
                const std::string &command = message.command(); // command
                const std::string &messageRaw = message.additional_data();  // param
                deliver->emitCommand("custom", {command, messageRaw});
                break;
        }
        server.sendReply();
    }
}

DeliverServerControllerImpl::~DeliverServerControllerImpl() = default;
