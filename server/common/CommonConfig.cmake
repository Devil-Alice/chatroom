cmake_minimum_required(VERSION 3.8)

# define project name


# 需要自己在包含次文件前定义的： set(PROJECT_NAME "app")

project(${PROJECT_NAME} C CXX)
add_definitions("-Wall -g")

message(STATUS "Importing common config: ${CMAKE_CURRENT_LIST_DIR}/CommonConfig.cmake")

# basic config:-----------------------------------------------------------------------
# This branch assumes that gRPC and all its dependencies are already installed
# on this system, so they can be located by find_package().

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(gRPC_DIR "/home/alice/app/grpc/lib/cmake/grpc")
set(Protobuf_DIR "/home/alice/app/grpc/lib/cmake/protobuf")
set(absl_DIR "/home/alice/app/grpc/lib/cmake/absl")
set(utf8_range_DIR "/home/alice/app/grpc/lib/cmake/utf8_range")

option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()
# --------------------------------------------------------------



# Include source files

# proto dir
set (PB_DIR ${CMAKE_CURRENT_LIST_DIR}/../proto_files)
message(STATUS "Include path add: ${PB_DIR}")
# proto files
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PB_DIR})
file(GLOB PB_SOURCES ${PB_DIR}/*.cc)

# common dir
set (COMMON_DIR ${CMAKE_CURRENT_LIST_DIR})
message(STATUS "Include path add: ${COMMON_DIR}")
# common files
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${COMMON_DIR})
file(GLOB COMMON_SOURCES ${COMMON_DIR}/*.cpp)

# source files
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${PB_SOURCES} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
absl::check
absl::flags
absl::flags_parse
absl::log
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
jsoncpp
hiredis
mysqlcppconn
)