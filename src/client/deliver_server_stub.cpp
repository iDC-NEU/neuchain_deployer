//
// Created by peng on 2021/6/8.
//

#include "client/deliver_server_stub.h"

DeliverServerStub::~DeliverServerStub() {

}

void DeliverServerStub::strReplace(const std::map<std::string, std::string> &replacement) {
    auto& googleReplacement = *cfgMsg->mutable_replacement();
    googleReplacement.clear();
    for(const auto& pair: replacement) {
        googleReplacement[pair.first] = pair.second;
    }
}

void DeliverServerStub::saveDockerComposeFile(const std::string &appName, const std::string &fileName) {
    *ctlMsg->mutable_folder() = appName;
    ctlMsg->set_status(docker_control_message_Status_CONFIG);
    sendAndResetMsg();
}

void DeliverServerStub::upDockerCompose(const std::string &appName) {
    *ctlMsg->mutable_folder() = appName;
    ctlMsg->set_status(docker_control_message_Status_UP);
    sendAndResetMsg();
}

void DeliverServerStub::downDockerCompose(const std::string &appName) {
    *ctlMsg->mutable_folder() = appName;
    ctlMsg->set_status(docker_control_message_Status_DOWN);
    sendAndResetMsg();
}

DeliverServerStub::DeliverServerStub(const std::string& serverIP)
        :client(std::make_unique<ZMQClient>(serverIP, "8888")),
         ctlMsg(std::make_unique<docker_control_message>()),
         cfgMsg(std::make_unique<docker_config_message>()) {

}

void DeliverServerStub::sendAndResetMsg() {
    *cfgMsg->mutable_raw_docker_compose_data() = fileData;
    *ctlMsg->mutable_additional_data() = cfgMsg->SerializeAsString();
    client->sendRequest(ctlMsg->SerializeAsString());
    ctlMsg = std::make_unique<docker_control_message>();
    cfgMsg = std::make_unique<docker_config_message>();
    client->getReply();
}
