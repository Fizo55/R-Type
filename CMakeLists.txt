cmake_minimum_required(VERSION 3.29)
project(server)

set(CMAKE_CXX_STANDARD 17)


add_library(engine-core.so OBJECT
    engine/test.cpp
)
target_compile_definitions(engine-core.so PUBLIC OBJ)
target_link_libraries(engine-core.so
    engine/test.cpp
)

add_executable(server-executable
    server/main.cpp
)

add_executable(client-executable
    client/main.cpp
)
