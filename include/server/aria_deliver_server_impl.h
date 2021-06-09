//
// Created by peng on 2021/6/9.
//

#ifndef DELIVER_SERVICE_ARIA_DELIVER_SERVER_IMPL_H
#define DELIVER_SERVICE_ARIA_DELIVER_SERVER_IMPL_H

#include "common/deliver_server.h"
#include "executor.h"
#include <vector>
#include <memory>

class AriaDeliverServerImpl: public IDockerComposeDeliverServer {
public:
    void strReplace(const std::map<std::string, std::string>& replacement) override;

    void setDockerComposeFile(const std::string&, const std::string&) override;

    void saveDockerComposeFile(const std::string&) override;

    void upDockerCompose(const std::string& appName) override;

    void downDockerCompose(const std::string&) override;

private:
    std::vector<std::unique_ptr<Executor>> pendingExecution;
};


#endif //DELIVER_SERVICE_ARIA_DELIVER_SERVER_IMPL_H
