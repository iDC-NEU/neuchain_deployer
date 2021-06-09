//
// Created by peng on 2021/6/8.
//

#include "server/deliver_server_impl.h"
#include "glog/logging.h"
#include <algorithm>
#include <fstream>

void DeliverServerImpl::strReplace(const std::map<std::string, std::string> &replacement) {
    auto stringReplace = [](std::string &strBig, const std::string &src, const std::string &dst) {
        std::string::size_type pos = 0;
        std::string::size_type src_len = src.size();
        std::string::size_type dst_len = dst.size();
        while((pos=strBig.find(src, pos)) != std::string::npos )
        {
            strBig.replace(pos, src_len, dst);
            pos += dst_len;
        }
    };

    for(const auto& pair: replacement) {
        stringReplace(fileData, pair.first, pair.second);
    }
    DLOG(INFO) << fileData;
}

void DeliverServerImpl::saveDockerComposeFile(const std::string &folderName) {
    Executor executor("mkdir " + folderName);
    executor.join();
    std::ofstream file(folderName + "/" + "docker-compose-modify.yaml", std::ios::out | std::ios::binary);
    file.write(fileData.data(), fileData.size());
    file.close();
}

void DeliverServerImpl::upDockerCompose(const std::string &folderName) {
    auto command = std::make_unique<Executor>(
            "cd " +
            folderName +
            ";\ndocker-compose -f docker-compose-modify.yaml up --force-recreate");
    pendingExecution.push_back(std::move(command));
}

void DeliverServerImpl::downDockerCompose(const std::string &folderName) {
    auto command = std::make_unique<Executor>(
            "cd " +
            folderName +
            ";\ndocker-compose -f docker-compose-modify.yaml down;\n"
            "(test -z \\\"$(docker ps -aq)\\\") || docker rm $(docker ps -aq);"
            "(test -z \\\"$(docker images dev* -q)\\\") || docker rmi $(docker images dev* -q);"
            "rm -rf /tmp/hfc-*\n"
            "docker volume rm $(docker volume ls -qf dangling=true)");
    pendingExecution.push_back(std::move(command));
}

DeliverServerImpl::~DeliverServerImpl() {
    for(auto& command: pendingExecution) {
        command->join();
    }
}
