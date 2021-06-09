//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_H
#define DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_H

#include <memory>
#include <functional>

class DeliverServerController {
public:
    virtual ~DeliverServerController() = default;

    virtual void run() = 0;

protected:
    std::unique_ptr<IDockerComposeDeliverServer> deliver{};
};


#endif //DELIVER_SERVICE_DELIVER_SERVER_CONTROLLER_H
