//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_IMPL_H
#define DELIVER_SERVICE_DELIVER_SERVER_IMPL_H

#include "deliver_server.h"
#include "executor.h"
#include <vector>
#include <memory>

class DeliverServerImpl: public IDockerComposeDeliverServer {
public:
    ~DeliverServerImpl() override;

    void strReplace(const std::map<std::string, std::string>& replacement) override;

    void saveDockerComposeFile(const std::string& folderName) override;

    void upDockerCompose(const std::string& folderName) override;

    void downDockerCompose(const std::string& folderName) override;

private:
    std::vector<std::unique_ptr<Executor>> pendingExecution;
};


#endif //DELIVER_SERVICE_DELIVER_SERVER_IMPL_H
