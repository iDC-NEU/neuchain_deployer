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
    explicit DeliverServerStub(const std::string& serverIP);
    ~DeliverServerStub() override;

    void strReplace(const std::map<std::string, std::string>& replacement) override;

    void saveDockerComposeFile(const std::string& appName) override;

    void upDockerCompose(const std::string& appName) override;

    void downDockerCompose(const std::string& appName) override;

protected:
    inline void sendAndResetMsg();

private:
    std::unique_ptr<ZMQClient> client;
    std::unique_ptr<docker_control_message> ctlMsg;
    std::unique_ptr<docker_config_message> cfgMsg;
};


#endif //DELIVER_SERVICE_DELIVER_SERVER_STUB_H
