#include <memory>
#include "server/deliver_server_impl.h"
#include "server/deliver_server_controller_impl.h"

int main() {
    DeliverServerControllerImpl controller;
    controller.setDeliverInstance(std::make_unique<DeliverServerImpl>());
    controller.run();
    return 0;
}
