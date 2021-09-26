//
// Created by peng on 2021/6/8.
//

#ifndef DELIVER_SERVICE_DELIVER_SERVER_H
#define DELIVER_SERVICE_DELIVER_SERVER_H

#include <string>
#include <google/protobuf/map.h>

/*
 *  This class is used in each server
 *  for recv command from client
 */

class IDockerComposeDeliverServer {
public:
    virtual ~IDockerComposeDeliverServer() = default;

    // util_interface(type, command, param); <- here
    //  -> type(command, param); <- will call
    //  -> command(param);
    virtual void emitCommand(const std::string &type, std::initializer_list<std::string> command) = 0;

    // type(command, param); <- here
    //  -> command(param); <- will call
    virtual void updateCommand(const std::string &command, const std::string &messageRaw) = 0;

    virtual void upCommand(const std::string &command) = 0;

    virtual void downCommand(const std::string &command) = 0;

protected:
    // load it from disk
    static bool loadConfigFile(std::string &data, const std::string &fileNameWithPath);

    static bool
    replaceConfigFileWithDict(std::string &data, const google::protobuf::Map<std::string, std::string> &dict);
};

#endif //DELIVER_SERVICE_DELIVER_SERVER_H
