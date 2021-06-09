cmake_minimum_required(VERSION 3.17)
project(deliver_service)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED on)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# 1. zmq
FIND_PACKAGE(cppzmq)

# 2. protobuf
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED "3.14.0")

if (Protobuf_FOUND)
    INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
else (Protobuf_FOUND)
    MESSAGE(FATAL_ERROR "PROTOBUF NOT FOUNED")
endif (Protobuf_FOUND)

LINK_DIRECTORIES(${PROTOBUF_LIBRARIES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(include)

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER
        include/proto/comm.proto
        )

ADD_LIBRARY(protobuf ${PROTO_HEADER} ${PROTO_SRC})

#3. yaml
FIND_PACKAGE(yaml-cpp REQUIRED)
INCLUDE_DIRECTORIES(${YAML_INCLUDE_DIRS})

FILE(GLOB SERVER_SRC_COMMON
        "src/server/executor.cpp"
        "src/server/deliver_server_controller_impl.cpp"
        "src/server/deliver_server_impl.cpp"
        "src/common/zmq/*.cpp"
        "src/deliver_server.cpp")

FILE(GLOB CLIENT_SRC_COMMON
        "src/client/deliver_server_stub.cpp"
        "src/client/deliver_server_client.cpp"
        "src/common/zmq/*.cpp"
        "src/deliver_server.cpp")

SET(LIBRARY_DEPENDENCIES
        ${PROTO_HEADER}
        ${PROTO_SRC}
        libyaml-cpp.a
        libprotobuf.a
        libzmq.a
        libgflags.a
        libglog.so
        )

add_executable(deliver_server
        ${SERVER_SRC_COMMON}
        src/server_main.cpp)

TARGET_LINK_LIBRARIES(deliver_server
        ${LIBRARY_DEPENDENCIES})

add_executable(deliver_client
        ${CLIENT_SRC_COMMON}
        src/client_main.cpp)

TARGET_LINK_LIBRARIES(deliver_client
        ${LIBRARY_DEPENDENCIES})