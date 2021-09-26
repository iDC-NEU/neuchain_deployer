//
// Created by peng on 2021/6/8.
//

#include "common/deliver_server.h"
#include <fstream>
#include "glog/logging.h"

bool IDockerComposeDeliverServer::loadConfigFile(std::string &data, const std::string &fileNameWithPath) {
    data.clear();
    std::ifstream file(fileNameWithPath, std::ios::in | std::ios::binary);
    if (!file.is_open() || !file.good()) {
        return false;
    }
    do {
        std::string buffer;
        buffer.resize(512);
        file.read(buffer.data(), buffer.capacity());
        int count = file.gcount();
        buffer.resize(count);
        data += buffer;
    } while (!file.eof());
    file.close();
    return true;
}

bool IDockerComposeDeliverServer::replaceConfigFileWithDict(std::string &data,
                                                            const google::protobuf::Map<std::string, std::string> &dict) {
    auto stringReplace = [](std::string &strBig, const std::string &src, const std::string &dst) {
        std::string::size_type pos = 0;
        std::string::size_type src_len = src.size();
        std::string::size_type dst_len = dst.size();
        while ((pos = strBig.find(src, pos)) != std::string::npos) {
            strBig.replace(pos, src_len, dst);
            pos += dst_len;
        }
    };
    for (const auto &pair: dict) {
        stringReplace(data, pair.first, pair.second);
    }
    DLOG(INFO) << data;
    return true;
}
