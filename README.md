# fabric_distributed_deployer
a c-s distributed deployer for hyperledger fabric

#### First: if init cc failed, pull the images
docker pull hyperledger/fabric-baseos:amd64-0.4.13
docker pull hyperledger/fabric-baseimage:amd64-0.4.13
docker pull hyperledger/fabric-ccenv:1.3.0
docker tag hyperledger/fabric-ccenv:1.3.0 hyperledger/fabric-ccenv:latest

#### 1. add local hosts in each peer ##########

#### 2. edit config file, modify ips and docker compose option ##########

#### 3. copy deliver_server into docker compose config folder, example: 4peer_1orderer_1org/deliver_server ##########

#### 4. modify setup.json, change ips

#### 5. in caliper-min/benchmark/smallbank dir
node ../../src/main.js -c fabric-config.json -n ../../../setup.json
