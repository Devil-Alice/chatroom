cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME "gate_server")
include(${CMAKE_CURRENT_SOURCE_DIR}/../common/CommonConfig.cmake)

project(${PROJECT_NAME} C CXX)

message(STATUS "Executing cmake: ${CMAKE_CURRENT_SOURCE_DIR}/CmakeLists.txt")
