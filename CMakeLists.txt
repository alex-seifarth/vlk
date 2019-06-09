cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(vlk
    VERSION 0.0.0
    DESCRIPTION "Support library for VULKAN tests and experiments - nothing for production use"
    LANGUAGES CXX
)

add_subdirectory(lib)

enable_testing()
add_subdirectory(test/utest)
