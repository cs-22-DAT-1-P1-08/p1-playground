cmake_minimum_required(VERSION 3.23)
project(p1_playground C)

set(CMAKE_C_STANDARD 99)

find_package(json-c)
find_package(pdcurses)

add_subdirectory(src)