//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_CLIENT_H
#define DELIVER_SERVICE_DELIVER_SERVER_CLIENT_H

#include <string>
#include <memory>
#include "deliver_server.h"
#include "deliver_server_controller.h"

class DeliverServerClient: public DeliverServerController {
public:
    DeliverServerClient() = default;
    ~DeliverServerClient() override;

    void run() override;

    std::function<std::unique_ptr<IDockerComposeDeliverServer>(const std::string&)> createDeliverServer;

protected:
    void serverUp();
    void updateConfigFile();
    void serverDown();

private:
    std::vector<std::unique_ptr<IDockerComposeDeliverServer>> deliverList;

};


#endif //DELIVER_SERVICE_DELIVER_SERVER_CLIENT_H
