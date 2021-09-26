//
// Created by peng on 2021/6/8.
//

#include "client/deliver_server_stub.h"

DeliverServerStub::DeliverServerStub(const std::string &serverIP)
        : client(std::make_unique<ZMQClient>(serverIP, "8888")),
          ctlMsg(std::make_unique<docker_control_message>()),
          cfgMsg(std::make_unique<docker_config_message>()) {

}

DeliverServerStub::~DeliverServerStub() = default;

void DeliverServerStub::updateCommand(const std::string &command, const std::string &messageRaw) {
    docker_config_message message;
    message.ParseFromString(messageRaw);
    cfgMsg->mutable_replacement()->swap(*message.mutable_replacement());
    ctlMsg->set_command(command);
    ctlMsg->set_status(docker_control_message_Status_CONFIG);
    sendAndResetMsg();
}

void DeliverServerStub::upCommand(const std::string &command) {
    ctlMsg->set_command(command);
    ctlMsg->set_status(docker_control_message_Status_UP);
    sendAndResetMsg();
}

void DeliverServerStub::downCommand(const std::string &command) {
    ctlMsg->set_command(command);
    ctlMsg->set_status(docker_control_message_Status_DOWN);
    sendAndResetMsg();
}

void DeliverServerStub::sendAndResetMsg() {
    ctlMsg->set_additional_data(cfgMsg->SerializeAsString());
    client->sendRequest(ctlMsg->SerializeAsString());
    client->getReply();
    ctlMsg->Clear();
    cfgMsg->Clear();
}

void DeliverServerStub::emitCommand(const std::string &type, std::initializer_list<std::string> command) {
    if (command.size()) {
        if (type == "up") {
            upCommand(*command.begin());
        }
        if (type == "down") {
            downCommand(*command.begin());
        }
        if (type == "update" && command.size() >= 2) {
            updateCommand(*command.begin(), *(command.begin() + 1));
        }
        if (type == "custom") {
            ctlMsg->set_command(*command.begin());  // no params
            ctlMsg->set_status(docker_control_message_Status_UNKNOWN);
            sendAndResetMsg();
        }
    }
}
