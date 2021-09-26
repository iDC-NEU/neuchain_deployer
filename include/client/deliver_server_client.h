//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_CLIENT_H
#define DELIVER_SERVICE_DELIVER_SERVER_CLIENT_H

#include <string>
#include <memory>
#include <yaml-cpp/yaml.h>
#include "common/deliver_server.h"
#include "common/deliver_server_controller.h"

class DeliverServerClient : public DeliverServerController {
public:
    DeliverServerClient();

    ~DeliverServerClient() override;

    void run() override;

    std::function<std::unique_ptr<IDockerComposeDeliverServer>(const std::string &)> createDeliverServer;

protected:
    void serverUp();

    void updateConfigFile();

    void serverDown();

    // broadcast to all servers, using command in yaml
    void customCommand();

    // send to specific server
    void customCommand(const std::string& ip, const std::string& command);

    // broadcast to all servers
    void customCommand(const std::string& command);

    std::string getNodeStringRecursively(const YAML::Node& node) const {
        return node.IsDefined() ? getReplacementValueRecursively(node.as<std::string>()) : std::string();
    }

    std::string getReplacementValueRecursively(const std::string &key) const;

private:
    std::vector<std::unique_ptr<IDockerComposeDeliverServer>> deliverList;
    const YAML::Node configNode;
};


#endif //DELIVER_SERVICE_DELIVER_SERVER_CLIENT_H
