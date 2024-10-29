add_library(network STATIC
    src/AsioNetwork.cpp
)

target_include_directories(network PUBLIC include)