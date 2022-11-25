cmake_minimum_required(VERSION 3.23)
project(p1_playground C)

set(CMAKE_C_STANDARD 99)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


find_package(json-c)
find_package(pdcurses)

add_subdirectory(src)
add_subdirectory(lib-examples)