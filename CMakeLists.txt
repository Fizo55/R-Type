cmake_minimum_required(VERSION 3.27)
project(rtype)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(raylib REQUIRED)
find_package(glfw3 REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(ENGINE_SRC_DIR ${CMAKE_SOURCE_DIR}/engine)
set(SERVER_SRC_DIR ${CMAKE_SOURCE_DIR}/server/src)
set(CLIENT_SRC_DIR ${CMAKE_SOURCE_DIR}/client)
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/r-type)

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


target_include_directories(engine-graphic PRIVATE ./include ${SDL2_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIRS})
target_compile_definitions(engine-graphic PUBLIC OBJ)
target_link_libraries(engine-graphic
PRIVATE
$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
$<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
$<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

add_library(engine-graphicRaylib OBJECT
    ${ENGINE_SRC_DIR}/raylib/wrapper/window.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/videoMode.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/texture.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/event.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/sprite.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/clock.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/windowHandler.cpp
    ${ENGINE_SRC_DIR}/raylib/displayManager.cpp
)

target_include_directories(engine-graphicRaylib PRIVATE ./include)
target_compile_definitions(engine-graphicRaylib PUBLIC OBJ)
target_link_libraries(engine-graphicRaylib PRIVATE raylib)
target_link_libraries(engine-graphicRaylib PRIVATE raylib glfw)

add_library(engine-core OBJECT
    ${ENGINE_SRC_DIR}/core/scene.cpp
    ${ENGINE_SRC_DIR}/core/game.cpp
    ${ENGINE_SRC_DIR}/core/entity.cpp
    ${ENGINE_SRC_DIR}/core/registry.cpp
    ${ENGINE_SRC_DIR}/core/object.cpp
    ${ENGINE_SRC_DIR}/core/components/position.cpp
    ${ENGINE_SRC_DIR}/core/components/size.cpp
    ${ENGINE_SRC_DIR}/core/components/sprite.cpp
    ${ENGINE_SRC_DIR}/core/components/script.cpp
    ${ENGINE_SRC_DIR}/core/components/volatile.cpp
)
target_include_directories(engine-core PRIVATE ./include)
target_compile_definitions(engine-core PUBLIC OBJ)
target_link_libraries(engine-core PUBLIC yaml-cpp::yaml-cpp)

add_library(engine-scripting OBJECT
    ${ENGINE_SRC_DIR}/scripting/bindings/game.cpp
    ${ENGINE_SRC_DIR}/scripting/bindings/object.cpp
    ${ENGINE_SRC_DIR}/scripting/bindings/any_list.cpp
    ${ENGINE_SRC_DIR}/scripting/bindings/orchestrator.cpp
    ${ENGINE_SRC_DIR}/scripting/ScriptEnvironment.cpp
    ${ENGINE_SRC_DIR}/scripting/ScriptOrchestrator.cpp
    ${ENGINE_SRC_DIR}/scripting/ScriptGlobalDefinition.cpp
)
target_include_directories(engine-scripting PRIVATE ./include)
target_include_directories(engine-scripting PRIVATE ${LUA_INCLUDE_DIR})
target_compile_definitions(engine-scripting PUBLIC OBJ)
target_link_libraries(engine-scripting PRIVATE ${LUA_LIBRARIES})

add_subdirectory(network)

add_executable(server-executable
    ${SERVER_SRC_DIR}/main.cpp
    ${SERVER_SRC_DIR}/ServerNetwork.cpp
    ${SERVER_SRC_DIR}/ArgumentParser.cpp
)

target_include_directories(server-executable PRIVATE ${CMAKE_SOURCE_DIR}/server/include)
target_include_directories(server-executable PRIVATE ./include)
target_link_libraries(server-executable PRIVATE engine-core network)

add_executable(client-executable
    ${CLIENT_SRC_DIR}/main_base.cpp
    ${CLIENT_SRC_DIR}/src/client_base.cpp
    ${ENGINE_SRC_DIR}/graphic/bindings/clock.cpp
    ${ENGINE_SRC_DIR}/graphic/bindings/displayManager.cpp
)
target_include_directories(client-executable PRIVATE ./include)
target_link_libraries(client-executable PRIVATE engine-graphic)
target_link_libraries(client-executable PRIVATE engine-core)
target_link_libraries(client-executable PRIVATE engine-scripting)
target_link_libraries(client-executable PUBLIC yaml-cpp::yaml-cpp)
target_link_libraries(client-executable PRIVATE ${LUA_LIBRARIES})

add_executable(client-secondary
    ${CLIENT_SRC_DIR}/main.cpp
    ${CLIENT_SRC_DIR}/src/client.cpp
)
target_include_directories(client-secondary PRIVATE ./include)
target_link_libraries(client-secondary PRIVATE engine-graphic)
target_link_libraries(client-secondary PRIVATE engine-core)
target_link_libraries(client-secondary PRIVATE engine-scripting)
target_link_libraries(client-secondary PUBLIC yaml-cpp::yaml-cpp)

add_custom_command(TARGET client-executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_DIR} $<TARGET_FILE_DIR:client-executable>/r-type
)

add_custom_command(TARGET client-executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/r-type
)

add_custom_command(TARGET client-secondary POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/secondary-game $<TARGET_FILE_DIR:client-secondary>/secondary-game
)

add_custom_command(TARGET client-secondary POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/secondary-game ${CMAKE_BINARY_DIR}/secondary-game
)