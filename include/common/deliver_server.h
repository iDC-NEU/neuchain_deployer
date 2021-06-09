//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_H
#define DELIVER_SERVICE_DELIVER_SERVER_H

#include <string>
#include <map>

/*
 *  This class is used in each server
 *  for recv command from client
 */

class IDockerComposeDeliverServer {
public:
    virtual ~IDockerComposeDeliverServer() = default;

    // directly pass data of a file
    virtual void setDockerComposeFile(const std::string& data) { fileData = data; }

    // load it from disk
    virtual void setDockerComposeFile(const std::string& folderName, const std::string& fileName);

    virtual void strReplace(const std::map<std::string, std::string>& replacement);

    virtual void saveDockerComposeFile(const std::string& appName) = 0;

    virtual void upDockerCompose(const std::string& appName) = 0;

    virtual void downDockerCompose(const std::string& appName) = 0;

protected:
    std::string fileData;
};

class IUserDeliverServer {
public:
    virtual ~IUserDeliverServer() = default;

    virtual void setSetupJsonFile(const std::string& data) = 0;

    virtual void connectZookeeper(const std::string& ip) = 0;

    virtual void setChaincodeFile(const std::string& data) = 0;

    virtual void upZookeeper() = 0;

    virtual void downZookeeper() = 0;
};

#endif //DELIVER_SERVICE_DELIVER_SERVER_H
