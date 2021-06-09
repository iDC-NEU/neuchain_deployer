//
// Created by peng on 2021/6/8.
//

#include "deliver_server.h"
#include <fstream>

void IDockerComposeDeliverServer::setDockerComposeFile(const std::string &folderName, const std::string &fileName) {
    fileData.clear();
    std::ifstream file(folderName + "/" + fileName, std::ios::in | std::ios::binary);
    do {
        std::string buffer;
        buffer.resize(512);
        file.read(buffer.data(), buffer.capacity());
        int count = file.gcount();
        buffer.resize(count);
        fileData += buffer;
    } while(!file.eof());
    file.close();
}
