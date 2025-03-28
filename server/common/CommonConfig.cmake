cmake_minimum_required(VERSION 3.8)

# !!!!!需要自己在包含次文件前定义的： set(PROJECT_NAME "project_name")!!!!!

project(${PROJECT_NAME} C CXX)
add_definitions("-Wall -g")

message(STATUS "Importing common config: ${CMAKE_CURRENT_LIST_DIR}/CommonConfig.cmake")

# ----------[basic config]----------
# basic config:
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
# ++++++++++[basic config]++++++++++


# add common source files

# ----------[my common source]----------
set (COMMON_DIR ${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${COMMON_DIR})
file(GLOB COMMON_SOURCES ${COMMON_DIR}/*.cpp)
message(STATUS "Common include path add: ${COMMON_DIR}")
# ++++++++++[my common source]++++++++++

# ----------[my boost utils source]----------
set (BOOST_UTILS_DIR ${CMAKE_CURRENT_LIST_DIR}/boost)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${BOOST_UTILS_DIR})
file(GLOB BOOST_UTILS_SOURCES ${BOOST_UTILS_DIR}/*.cpp)
message(STATUS "Common include path add: ${BOOST_UTILS_DIR}")
# ++++++++++[my boost utils source]++++++++++

# ----------[my grpc client source]----------
set (GRPC_CLIENT_DIR ${CMAKE_CURRENT_LIST_DIR}/grpc_client)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GRPC_CLIENT_DIR})
file(GLOB GRPC_CLIENT_SOURCES ${GRPC_CLIENT_DIR}/*.cpp)
message(STATUS "Common include path add: ${GRPC_CLIENT_DIR}")
# ++++++++++[my grpc client source]++++++++++

# ----------[my lib source]----------
set (MY_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/my_lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${MY_LIB_DIR})
file(GLOB MY_LIB_SOURCES ${MY_LIB_DIR}/*.cpp)
message(STATUS "Common include path add: ${MY_LIB_DIR}")
# ++++++++++[my lib source]++++++++++

# ----------[my mysql manager source]----------
set (MYSQL_MANAGER_DIR ${CMAKE_CURRENT_LIST_DIR}/mysql)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${MYSQL_MANAGER_DIR})
file(GLOB MYSQL_MANAGER_SOURCES ${MYSQL_MANAGER_DIR}/*.cpp)
message(STATUS "Common include path add: ${MYSQL_MANAGER_DIR}")
# ++++++++++[my mysql manager source]++++++++++

# ----------[my proto source]----------
set (PB_DIR ${CMAKE_CURRENT_LIST_DIR}/proto_files)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PB_DIR})
file(GLOB PB_SOURCES ${PB_DIR}/*.cc)
message(STATUS "Common include path add: ${PB_DIR}")
# ++++++++++[my proto source]++++++++++

# ----------[my redis manager source]----------
set (REDIS_MANAGER_DIR ${CMAKE_CURRENT_LIST_DIR}/redis)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${REDIS_MANAGER_DIR})
file(GLOB REDIS_MANAGER_SOURCES ${REDIS_MANAGER_DIR}/*.cpp)
message(STATUS "Common include path add: ${REDIS_MANAGER_DIR}")
# ++++++++++[my redis manager source]++++++++++

# ----------[my xxx]----------
# ++++++++++[my xxx]++++++++++


# source files
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES}
${COMMON_SOURCES} 
${BOOST_UTILS_SOURCES}
${GRPC_CLIENT_SOURCES}
${MY_LIB_SOURCES}
${MYSQL_MANAGER_SOURCES}
${PB_SOURCES} 
${REDIS_MANAGER_SOURCES}
)


target_link_libraries(
${PROJECT_NAME}
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

# copy configuration file
configure_file(${CMAKE_CURRENT_LIST_DIR}/app_config.json ${CMAKE_BINARY_DIR}/app_config.json COPYONLY)