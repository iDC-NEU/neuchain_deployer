//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_EXECUTOR_H
#define DELIVER_SERVICE_EXECUTOR_H

#include <string>

class Executor {
public:
    explicit Executor(std::string command);

    ~Executor();

    void join(bool print = true);

    bool status() { return pp != nullptr; }

private:
    void *pp;
};

#endif //DELIVER_SERVICE_EXECUTOR_H
