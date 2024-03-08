cmake_minimum_required(VERSION 3.15)
project(chatClient)

set(CMAKE_CXX_STANDARD 14)

add_executable(chatClient main.cpp source/write_handler.cpp headers/write_handler.h source/write_commands.cpp headers/write_commands.h source/read_handler.cpp headers/read_handler.h source/read_commands.cpp headers/read_commands.h)