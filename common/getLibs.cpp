/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** getLibs
*/

#include <vector>
#include <memory>
#include <dlfcn.h>
#include <iostream>
#include <dirent.h>
#include "libs.hpp"

static std::unique_ptr<GraphLib> getGraphLib(int ac, char **av)
{
    void *handle = nullptr;
    std::unique_ptr<GraphLib> graphlib;
    for (int i = 0; av[i]; i++) {
        if (av[i] == "-g") {
            handle = dlopen(av[i + 1], RTLD_LAZY);
        }
    }
    if (handle == nullptr){
        std::cerr << "failed to open a graphic lib, check if you used the good filepath after the -g flag." << std::endl;
        return (nullptr);
    }
    //todo check magic number
    return (graphlib);
}

static std::unique_ptr<AudioLib> getAudioLib(int ac, char **av)
{
    void *handle = nullptr;
    std::unique_ptr<AudioLib> audiolib;
    for (int i = 0; av[i]; i++) {
        if (av[i] == "-a") {
            handle = dlopen(av[i + 1], RTLD_LAZY);
        }
    }
    if (handle == nullptr){
        std::cerr << "failed to open a audio lib, check if you used the good filepath after the -a flag." << std::endl;
        return (nullptr);
    }
    //todo check magic number
    return (audiolib);
}

static std::unique_ptr<NetworkLib> getNetworkLib(int ac, char **av)
{
    void *handle = nullptr;
    std::unique_ptr<NetworkLib> networklib;
    for (int i = 0; av[i]; i++) {
        if (av[i] == "-n") {
            handle = dlopen(av[i + 1], RTLD_LAZY);
        }
    }
    if (handle == nullptr){
        std::cerr << "failed to open a network lib, check if you used the good filepath after the -n flag." << std::endl;
        return (nullptr);
    }
    //todo check magic number
    return (networklib);
}

static std::unique_ptr<CoreLib> getCoreLib(int ac, char **av)
{
    void *handle = nullptr;
    std::unique_ptr<CoreLib> corelib;
    for (int i = 0; av[i]; i++) {
        if (av[i] == "-c") {
            handle = dlopen(av[i + 1], RTLD_LAZY);
        }
    }
    if (handle == nullptr){
        std::cerr << "failed to open a core lib, check if you used the good filepath after the -c flag." << std::endl;
        return (nullptr);
    }
    //todo check magic number
    return (corelib);
}

static std::unique_ptr<AlgoLib> getAlgoLib(int ac, char **av)
{
    void *handle = nullptr;
    std::unique_ptr<AlgoLib> algolib;
    for (int i = 0; av[i]; i++) {
        if (av[i] == "-l") {
            handle = dlopen(av[i + 1], RTLD_LAZY);
        }
    }
    if (handle == nullptr){
        std::cerr << "failed to open a algo lib, check if you used the good filepath after the -l flag." << std::endl;
        return (nullptr);
    }
    //todo check magic number
    return (algolib);
}

std::vector<std::unique_ptr<ILib>> getLibs(std::string type, int ac, char **av)
{
    std::vector<std::unique_ptr<ILib>> libs;
    std::unique_ptr<ILib> lib;

    lib = getCoreLib(ac, av);
    if (lib != nullptr)
        libs.push_back(std::move(lib));
    lib = getAlgoLib(ac, av);
    if (lib != nullptr)
        libs.push_back(std::move(lib));
    lib = getNetworkLib(ac, av);
    if (lib != nullptr)
        libs.push_back(std::move(lib));
    if (type == "client") {
        lib = getGraphLib(ac, av);
        if (lib != nullptr)
            libs.push_back(std::move(lib));
        lib = getAudioLib(ac, av);
        if (lib != nullptr)
            libs.push_back(std::move(lib));
    }
    return (libs);
}