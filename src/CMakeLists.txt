cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME "chatroom")
project(${PROJECT_NAME} C CXX)
add_definitions("-Wall -g")

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
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../proto_files)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB PBSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../proto_files/*.cc)
add_executable(${PROJECT_NAME} ${SOURCES} ${PBSOURCES})

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
)