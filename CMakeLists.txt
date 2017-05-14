cmake_minimum_required(VERSION 3.6)
project(Chat)

set(CMAKE_C_STANDARD 99)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

find_package(Ice 3.4.0 REQUIRED Ice IceUtil Glacier2)
include_directories (${Ice_INCLUDE_DIRS})

set(CLIENT_FILES Client.cpp chat.cpp chat.h chatI.cpp chatI.h)
add_executable(client ${CLIENT_FILES})
target_link_libraries (client ${Ice_LIBRARIES} pthread)

set(GROUP_SERVER_MANAGER_FILES ServerManager.cpp chat.cpp chat.h chatI.cpp chatI.h)
add_executable(group_server_manager ${GROUP_SERVER_MANAGER_FILES})
target_link_libraries (group_server_manager ${Ice_LIBRARIES} pthread)

set(CHAT_SERVER_FILES Server.cpp chat.cpp chat.h chatI.cpp chatI.h)
add_executable(chat_server ${CHAT_SERVER_FILES})
target_link_libraries (chat_server ${Ice_LIBRARIES} pthread)