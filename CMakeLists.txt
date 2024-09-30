cmake_minimum_required(VERSION 3.29)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} `pkg-config --cflags --libs sdl2 SDL2_image`")

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

include_directories(${ASIO_INCLUDE_DIR})

set(ENGINE_SRC_DIR ${CMAKE_SOURCE_DIR}/engine)
set(SERVER_SRC_DIR ${CMAKE_SOURCE_DIR}/server)
set(CLIENT_SRC_DIR ${CMAKE_SOURCE_DIR}/client)

add_library(engine-graphic OBJECT
    ${ENGINE_SRC_DIR}/graphic/wrapper/window.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/videoMode.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/texture.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/event.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/clock.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/mask.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/windowHandler.cpp
)

target_include_directories(engine-graphic PRIVATE ./include)
target_compile_definitions(engine-graphic PUBLIC OBJ)
target_link_libraries(engine-graphic PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image)

add_library(engine-core OBJECT
    ${ENGINE_SRC_DIR}/core/scene.cpp
    ${ENGINE_SRC_DIR}/core/game.cpp
    ${ENGINE_SRC_DIR}/core/entity.cpp
    ${ENGINE_SRC_DIR}/core/registry.cpp
)
target_include_directories(engine-core PRIVATE ./include)
target_compile_definitions(engine-core PUBLIC OBJ)
target_link_libraries(engine-core PUBLIC yaml-cpp)

add_executable(server-executable
    ${SERVER_SRC_DIR}/main.cpp
)
target_include_directories(server-executable PRIVATE ./include)
target_link_libraries(server-executable PRIVATE engine-core)
target_include_directories(server-executable PRIVATE ${SERVER_SRC_DIR}/udp)
target_link_libraries(server-executable PRIVATE engine-graphic)

add_executable(client-executable
    ${CLIENT_SRC_DIR}/main.cpp
)

target_include_directories(client-executable PRIVATE ./include ${CLIENT_SRC_DIR}/include ${ENGINE_SRC_DIR}/graphic/wrapper)
target_link_libraries(client-executable PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image)
