//
// Created by peng on 2021/6/8.
//

#include "common/deliver_server.h"
#include <fstream>
#include "glog/logging.h"

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

void IDockerComposeDeliverServer::strReplace(const std::map<std::string, std::string> &replacement) {
    auto stringReplace = [](std::string &strBig, const std::string &src, const std::string &dst) {
        std::string::size_type pos = 0;
        std::string::size_type src_len = src.size();
        std::string::size_type dst_len = dst.size();
        while((pos=strBig.find(src, pos)) != std::string::npos) {
            strBig.replace(pos, src_len, dst);
            pos += dst_len;
        }
    };

    for(const auto& pair: replacement) {
        stringReplace(fileData, pair.first, pair.second);
    }
    DLOG(INFO) << fileData;
}
