//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_STUB_H
#define DELIVER_SERVICE_DELIVER_SERVER_STUB_H

#include "common/deliver_server.h"
#include <memory>
#include "common/zmq/zmq_client.h"
#include "comm.pb.h"

class DeliverServerStub : public IDockerComposeDeliverServer {
public:
    explicit DeliverServerStub(const std::string &serverIP);

    ~DeliverServerStub() override;

    void emitCommand(const std::string &type, std::initializer_list<std::string> command) override;

    void updateCommand(const std::string &command, const std::string &messageRaw) override;

    void upCommand(const std::string &command) override;

    void downCommand(const std::string &command) override;

protected:
    void sendAndResetMsg();

private:
    std::unique_ptr<ZMQClient> client;
    std::unique_ptr<docker_control_message> ctlMsg;
    std::unique_ptr<docker_config_message> cfgMsg;
};


#endif //DELIVER_SERVICE_DELIVER_SERVER_STUB_H
