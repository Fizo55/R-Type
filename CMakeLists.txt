cmake_minimum_required(VERSION 3.29)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENGINE_SRC_DIR ${CMAKE_SOURCE_DIR}/engine)
set(SERVER_SRC_DIR ${CMAKE_SOURCE_DIR}/server)
set(CLIENT_SRC_DIR ${CMAKE_SOURCE_DIR}/client)

add_library(engine-core OBJECT
    ${ENGINE_SRC_DIR}/test.cpp
)

target_compile_definitions(engine-core PUBLIC OBJ)

add_executable(server-executable
    ${SERVER_SRC_DIR}/main.cpp
)

add_executable(client-executable
    ${CLIENT_SRC_DIR}/main.cpp
)

target_link_libraries(server-executable PRIVATE engine-core)
target_link_libraries(client-executable PRIVATE engine-core)