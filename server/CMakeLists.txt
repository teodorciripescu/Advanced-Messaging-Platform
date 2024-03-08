cmake_minimum_required(VERSION 3.15)
project(reteleChatServer)

set(CMAKE_CXX_STANDARD 14)


add_executable(reteleChatServer main.cpp source/database.cpp headers/database.h source/server_boilerplate.cpp headers/server_boilerplate.h source/database_commands.cpp headers/database_commands.h source/command_handler.cpp headers/command_handler.h)
target_link_libraries(reteleChatServer LINK_PUBLIC sqlite3)