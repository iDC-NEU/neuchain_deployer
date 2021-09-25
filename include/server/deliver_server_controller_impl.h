//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_IMPL_H
#define DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_IMPL_H

#include <string>
#include <memory>
#include "common/deliver_server.h"
#include "common/deliver_server_controller.h"

class DeliverServerControllerImpl : public DeliverServerController {
public:
    ~DeliverServerControllerImpl() override;

    void run() override;

    inline void setDeliverInstance(IDockerComposeDeliverServer *ptr) { deliver.reset(ptr); }
};


#endif //DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_IMPL_H
