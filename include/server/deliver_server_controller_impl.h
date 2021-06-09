//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_IMPL_H
#define DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_IMPL_H

#include <string>
#include <memory>
#include "deliver_server.h"
#include "deliver_server_controller.h"

class DeliverServerControllerImpl: public DeliverServerController {
public:
    DeliverServerControllerImpl() = default;
    ~DeliverServerControllerImpl() override;

    inline void setDeliverInstance(std::unique_ptr<IDockerComposeDeliverServer> ptr) { deliver = std::move(ptr); }

    void run() override;

protected:
    void configDockerContainer(const std::string& commandRaw, const std::string& folder);
    void startDockerContainer(const std::string& folder);
    void stopDockerContainer(const std::string& folder);
};


#endif //DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_IMPL_H
