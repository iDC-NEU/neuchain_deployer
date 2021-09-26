# this readme may be out-of-date, just for reference
# aria 初始化步骤-------------------------
apt update ; apt install unzip; unzip release_0.20_v2.zip; cd release_0.20; ./install.sh; ./aria_deliver_server

apt update ; apt install unzip; unzip release_0.20_v2.zip; cd release_0.20; ./install.sh; ./deliver_client


# fabric 网络初始化步骤-------------------
# 1. init dep
apt update ; apt install unzip git docker docker-compose nload -y; unzip 4p3o_v4.zip; cd 4p3o

# 2. pull image
docker pull hyperledger/fabric-baseos:amd64-0.4.22
docker pull hyperledger/fabric-ccenv:1.4.0
docker pull hyperledger/fabric-orderer:1.4.0
docker pull hyperledger/fabric-peer:1.4.0
docker pull hyperledger/fabric-kafka:latest
docker pull hyperledger/fabric-zookeeper:latest
docker pull hyperledger/fabric-ca:1.5.0
docker tag hyperledger/fabric-ccenv:1.4.0 hyperledger/fabric-ccenv:latest

# 3. start the server
./install.sh

# caliper 测试初始化步骤------------------
# for server
apt update ; apt install unzip git docker docker-compose npm nload mosquitto -y; unzip 4p3o_v3.zip; cd 4p3o/caliper-min; npm install; cd ..; ./deliver_server

# edit the config file
# in the new window
./deliver_client

# after start the network and update the config
# for zoo communicator
export ZOO_0_IP=172.26.160.211
docker-compose -f zoo0/docker-compose.yaml up
node caliper-min/src/comm/client/zoo-client.js 172.26.160.211:2181

# benchmark in main dic
node caliper-min/src/main.js -c caliper_config/benchmark.json -n caliper_config/network_modify.json



# fabric 自定义镜像步骤（以替换peer为例）------------
# 1. init dep
apt update ; apt install unzip git docker docker-compose nload -y; unzip 4p1o_v5.zip; docker load < peer-14-with-log.tar; cd 4p1o_v5

# 2. pull image
docker pull hyperledger/fabric-baseos:amd64-0.4.22
docker pull hyperledger/fabric-ccenv:1.4
docker pull hyperledger/fabric-orderer:1.4.0
docker pull hyperledger/fabric-kafka:latest
docker pull hyperledger/fabric-zookeeper:latest
docker pull hyperledger/fabric-ca:1.5.0
docker tag hyperledger/fabric-ccenv:1.4 hyperledger/fabric-ccenv:latest
docker tag hyperledger/fabric-peer:1.4.12 hyperledger/fabric-peer:1.4.0

# 3. start the server
./deliver_server




# fabric 自定义镜像步骤（以替换peer& orderer为例）------------
# 1. init dep
# apt update ; apt install unzip git docker docker-compose nload -y; unzip 4p1o_v5.zip; docker load < peer-14-with-log.tar; docker load < orderer-log.tar; cd 4p1o_v5
apt update ; apt install unzip git docker docker-compose nload -y; unzip 4p1o_v5.zip; docker load < fast-peer-log.tar; docker load < orderer-log.tar; cd 4p1o_v5

# 2. pull image
unzip 4p3o_v3.zip; cd 4p3o_v3
docker pull hyperledger/fabric-baseos:amd64-0.4.22
docker pull hyperledger/fabric-ccenv:1.4
docker pull hyperledger/fabric-kafka:latest
docker pull hyperledger/fabric-zookeeper:latest
docker pull hyperledger/fabric-ca:1.5.0
docker tag hyperledger/fabric-ccenv:1.4 hyperledger/fabric-ccenv:latest

# docker tag hyperledger/fabric-peer:1.4.12 hyperledger/fabric-peer:1.4.0
docker tag hyperledger/fastfabric-peer:1.4.7 hyperledger/fabric-peer:1.4.0

docker tag hyperledger/fabric-orderer:1.4.12 hyperledger/fabric-orderer:1.4.0

# 3. start the server
./deliver_server



coredump:
mkdir /root/core
ulimit -c unlimited
sysctl -w kernel.core_pattern=/root/core/core.%e.%p


