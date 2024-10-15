cmake_minimum_required(VERSION 3.16)
project(rtype)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.26.5
)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
  SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-2.6.2
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_TESTS OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL2_SHARED OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_SDL2_SHARED OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_image)

FetchContent_Declare(
  yaml_cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.7.0
)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(yaml_cpp)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-24-0
)
FetchContent_MakeAvailable(asio)

find_package(Threads REQUIRED)

set(ENGINE_SRC_DIR ${CMAKE_SOURCE_DIR}/engine)
set(SERVER_SRC_DIR ${CMAKE_SOURCE_DIR}/server)
set(CLIENT_SRC_DIR ${CMAKE_SOURCE_DIR}/client)
set(ENGINE_INCLUDE_DIR ${ENGINE_SRC_DIR}/include)
set(SERVER_INCLUDE_DIR ${SERVER_SRC_DIR}/include)
set(CLIENT_INCLUDE_DIR ${CLIENT_SRC_DIR}/include)

add_library(engine-graphic STATIC
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

target_include_directories(engine-graphic PUBLIC
    ${ENGINE_INCLUDE_DIR}
    $<TARGET_PROPERTY:SDL2::SDL2-static,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:SDL2_image::SDL2_image,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(engine-graphic PUBLIC
    SDL2::SDL2-static
    SDL2_image::SDL2_image
)

add_library(engine-core STATIC
    ${ENGINE_SRC_DIR}/core/scene.cpp
    ${ENGINE_SRC_DIR}/core/game.cpp
    ${ENGINE_SRC_DIR}/core/entity.cpp
    ${ENGINE_SRC_DIR}/core/registry.cpp
    ${ENGINE_SRC_DIR}/core/object.cpp
    ${ENGINE_SRC_DIR}/core/components/position.cpp
    ${ENGINE_SRC_DIR}/core/components/sprite.cpp
)

target_include_directories(engine-core PUBLIC
    ${ENGINE_INCLUDE_DIR}
    $<TARGET_PROPERTY:yaml-cpp,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(engine-core PUBLIC
    yaml-cpp
)

file(GLOB_RECURSE SERVER_SOURCES
    ${SERVER_SRC_DIR}/src/*.cpp
)

add_executable(server-executable
    ${SERVER_SOURCES}
)

target_include_directories(server-executable PRIVATE
    ${SERVER_INCLUDE_DIR}
    ${ENGINE_INCLUDE_DIR}
    ${asio_SOURCE_DIR}/asio/include
)

target_compile_definitions(server-executable PRIVATE ASIO_STANDALONE)

target_link_libraries(server-executable PRIVATE
    engine-core
    Threads::Threads
)

file(GLOB_RECURSE CLIENT_SOURCES
    ${CLIENT_SRC_DIR}/src/*.cpp
)

add_executable(client-executable
    ${CLIENT_SRC_DIR}/main.cpp
    ${CLIENT_SRC_DIR}/src/client.cpp
)

target_include_directories(client-executable PRIVATE
    ${CLIENT_INCLUDE_DIR}
    ${ENGINE_INCLUDE_DIR}
)

target_link_libraries(client-executable PRIVATE
    engine-core
    engine-graphic
    SDL2::SDL2-static
    SDL2_image::SDL2_image
    Threads::Threads
)
