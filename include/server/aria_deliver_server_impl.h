//
// Created by peng on 2021/6/9.
//

#ifndef DELIVER_SERVICE_ARIA_DELIVER_SERVER_IMPL_H
#define DELIVER_SERVICE_ARIA_DELIVER_SERVER_IMPL_H

#include "common/deliver_server.h"
#include "executor.h"
#include <vector>
#include <memory>

class AriaDeliverServerImpl : public IDockerComposeDeliverServer {
public:
    void emitCommand(const std::string &type, std::initializer_list<std::string> command) override;

    void updateCommand(const std::string &command, const std::string &messageRaw) override;

    void upCommand(const std::string &command) override;

    void downCommand(const std::string &command) override;

private:
    std::vector<std::unique_ptr<Executor>> pendingExecution;
};


#endif //DELIVER_SERVICE_ARIA_DELIVER_SERVER_IMPL_H
