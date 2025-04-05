cmake_minimum_required(VERSION 3.8)

project(chatroom)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gate_server)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/grpc_server/verify_server)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/grpc_server/status_server)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/chat_server)
