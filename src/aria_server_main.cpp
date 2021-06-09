#include <memory>
#include "server/aria_deliver_server_impl.h"
#include "server/deliver_server_controller_impl.h"

int main() {
    DeliverServerControllerImpl controller;
    controller.setDeliverInstance(std::make_unique<AriaDeliverServerImpl>());
    controller.run();
    return 0;
}
