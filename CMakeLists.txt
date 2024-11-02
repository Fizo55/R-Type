cmake_minimum_required(VERSION 3.27)
project(rtype)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(raylib REQUIRED)
find_package(glfw3 REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(ENGINE_SRC_DIR ${CMAKE_SOURCE_DIR}/engine)
set(SERVER_SRC_DIR ${CMAKE_SOURCE_DIR}/server/src)
set(CLIENT_SRC_DIR ${CMAKE_SOURCE_DIR}/client)
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/r-type)

add_library(engine-graphicSDL OBJECT
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


target_include_directories(engine-graphicSDL PRIVATE ./include ${SDL2_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIRS})
target_compile_definitions(engine-graphicSDL PUBLIC OBJ)
target_link_libraries(engine-graphicSDL
PRIVATE
$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
$<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)

add_library(engine-graphic OBJECT
    ${ENGINE_SRC_DIR}/raylib/wrapper/window.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/videoMode.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/texture.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/event.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/sprite.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/clock.cpp
    ${ENGINE_SRC_DIR}/raylib/wrapper/windowHandler.cpp
    ${ENGINE_SRC_DIR}/raylib/displayManager.cpp
)

target_include_directories(engine-graphic PRIVATE ./include)
target_compile_definitions(engine-graphic PUBLIC OBJ)
target_link_libraries(engine-graphic PRIVATE raylib)
target_link_libraries(engine-graphic PRIVATE raylib glfw)



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
    ${SERVER_SRC_DIR}/MessageBase.cpp
    ${SERVER_SRC_DIR}/BinaryDeserializer.cpp
    ${SERVER_SRC_DIR}/BinarySerializer.cpp
    ${SERVER_SRC_DIR}/ConsoleLogger.cpp
    ${SERVER_SRC_DIR}/GameStateUpdateMessage.cpp
    ${SERVER_SRC_DIR}/ServerWelcomeMessage.cpp
    ${SERVER_SRC_DIR}/ClientHelloMessage.cpp
    ${SERVER_SRC_DIR}/AcknowledgmentMessage.cpp
    ${SERVER_SRC_DIR}/MessageDispatcher.cpp
    ${SERVER_SRC_DIR}/MessageFactory.cpp
    ${SERVER_SRC_DIR}/PlayerInputMessage.cpp
    ${SERVER_SRC_DIR}/RTypeGameLogic.cpp
    ${SERVER_SRC_DIR}/RTypeServer.cpp
    ${SERVER_SRC_DIR}/ThreadPoolScheduler.cpp
    ${SERVER_SRC_DIR}/ArgumentParser.cpp
)

target_include_directories(server-executable PRIVATE ${CMAKE_SOURCE_DIR}/server/include)
target_include_directories(server-executable PRIVATE ./include)
target_link_libraries(server-executable PRIVATE engine-core network)

add_executable(client-executable
    ${CLIENT_SRC_DIR}/main.cpp
    ${CLIENT_SRC_DIR}/src/client.cpp
)
target_include_directories(client-executable PRIVATE ./include)
target_link_libraries(client-executable PRIVATE engine-graphic)
target_link_libraries(client-executable PRIVATE engine-core)
target_link_libraries(client-executable PRIVATE engine-scripting)
target_link_libraries(client-executable PUBLIC yaml-cpp::yaml-cpp)

add_custom_command(TARGET client-executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_DIR} $<TARGET_FILE_DIR:client-executable>/r-type
)

add_custom_command(TARGET client-executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/r-type
)