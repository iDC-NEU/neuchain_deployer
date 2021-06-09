#include <memory>
#include "client/deliver_server_client.h"
#include "client/deliver_server_stub.h"

int main() {
    DeliverServerClient controller;
    controller.createDeliverServer = [](const std::string& ip) -> std::unique_ptr<IDockerComposeDeliverServer> {
        return std::make_unique<DeliverServerStub>(ip);
    };
    controller.run();
    return 0;
}
