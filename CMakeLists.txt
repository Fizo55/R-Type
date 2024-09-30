cmake_minimum_required(VERSION 3.29)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
target_link_libraries(engine-graphic PRIVATE SDL2)
target_link_libraries(engine-graphic PRIVATE SDL2_image)

add_executable(server-executable
    ${SERVER_SRC_DIR}/main.cpp
    ${SERVER_SRC_DIR}/src/Abstracts/AEntity.cpp
    ${SERVER_SRC_DIR}/src/Utility/Timer.cpp
    ${SERVER_SRC_DIR}/src/Utility/Position.cpp
    ${SERVER_SRC_DIR}/src/Utility/Movement.cpp
    ${SERVER_SRC_DIR}/src/Utility/Acceleration.cpp
)
target_include_directories(server-executable PRIVATE ./server/includes)

add_executable(client-executable
    ${CLIENT_SRC_DIR}/main.cpp
)
target_include_directories(client-executable PRIVATE ./include)

target_link_libraries(client-executable PRIVATE engine-graphic)