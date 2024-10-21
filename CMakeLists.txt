cmake_minimum_required(VERSION 3.27)
project(server)

if(NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/cmake/CPM_cache")
endif()

if(NOT IS_DIRECTORY "${CPM_SOURCE_CACHE}")
    file(MAKE_DIRECTORY "${CPM_SOURCE_CACHE}")
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/cmake")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/cmake")
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
    message(STATUS "Downloading CPM.cmake to ${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/cpm.cmake
        "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake"
        SHOW_PROGRESS
    )

    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
        message(FATAL_ERROR "Failed to download CPM.cmake!")
    endif()
endif()

include("${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

CPMAddPackage(
    NAME ASIO
    GITHUB_REPOSITORY chriskohlhoff/asio
    GIT_TAG master
)

CPMAddPackage(
    NAME SDL2
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.30.7
)

CPMAddPackage(
    NAME SDL2_image
    GITHUB_REPOSITORY libsdl-org/SDL_image
    GIT_TAG release-2.8.2
)

CPMAddPackage(
    NAME yaml-cpp
    GITHUB_REPOSITORY jbeder/yaml-cpp
    GIT_TAG master
)

CPMAddPackage(
    NAME luacpp
    GITHUB_REPOSITORY jordanvrtanoski/luacpp
    GIT_TAG master
)

set(ENGINE_SRC_DIR ${CMAKE_SOURCE_DIR}/engine)
set(SERVER_SRC_DIR ${CMAKE_SOURCE_DIR}/server/src)
set(CLIENT_SRC_DIR ${CMAKE_SOURCE_DIR}/client)

add_library(engine-graphic OBJECT
    ${ENGINE_SRC_DIR}/graphic/wrapper/window.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/videoMode.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/texture.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/event.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/sprite.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/clock.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/mask.cpp
    ${ENGINE_SRC_DIR}/graphic/wrapper/windowHandler.cpp
    ${ENGINE_SRC_DIR}/graphic/displayManager.cpp
)

target_include_directories(engine-graphic PRIVATE ./include ${SDL2_SOURCE_DIR}/include ${SDL2_image_SOURCE_DIR}/include)
target_compile_definitions(engine-graphic PUBLIC OBJ)
target_link_libraries(engine-graphic PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)

add_library(engine-core OBJECT
    ${ENGINE_SRC_DIR}/core/scene.cpp
    ${ENGINE_SRC_DIR}/core/game.cpp
    ${ENGINE_SRC_DIR}/core/entity.cpp
    ${ENGINE_SRC_DIR}/core/registry.cpp
    ${ENGINE_SRC_DIR}/core/object.cpp
    ${ENGINE_SRC_DIR}/core/components/position.cpp
    ${ENGINE_SRC_DIR}/core/components/sprite.cpp
)
target_include_directories(engine-core PRIVATE ./include)
target_compile_definitions(engine-core PUBLIC OBJ)
target_link_libraries(engine-core PUBLIC yaml-cpp)

add_executable(server-executable
    ${SERVER_SRC_DIR}/main.cpp
    ${SERVER_SRC_DIR}/AsioNetwork.cpp
    ${SERVER_SRC_DIR}/BinaryDeserializer.cpp
    ${SERVER_SRC_DIR}/BinarySerializer.cpp
    ${SERVER_SRC_DIR}/ConsoleLogger.cpp
    ${SERVER_SRC_DIR}/GameStateUpdateMessage.cpp
    ${SERVER_SRC_DIR}/MessageDispatcher.cpp
    ${SERVER_SRC_DIR}/MessageFactory.cpp
    ${SERVER_SRC_DIR}/PlayerInputMessage.cpp
    ${SERVER_SRC_DIR}/RTypeGameLogic.cpp
    ${SERVER_SRC_DIR}/Server.cpp
    ${SERVER_SRC_DIR}/ThreadPoolScheduler.cpp
)

target_include_directories(server-executable PRIVATE ${CMAKE_SOURCE_DIR}/server/include)
target_link_libraries(server-executable PRIVATE engine-core)
target_link_libraries(server-executable PRIVATE asio)

add_executable(client-executable
    ${CLIENT_SRC_DIR}/main.cpp
    ${CLIENT_SRC_DIR}/src/client.cpp
)
target_include_directories(client-executable PRIVATE ./include)
target_link_libraries(client-executable PRIVATE engine-graphic)
target_link_libraries(client-executable PRIVATE engine-core)