cmake_minimum_required(VERSION 3.23)
project(p1_playground C)

set(CMAKE_C_STANDARD 99)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(json-c)
find_package(pdcurses)

# TODO: Find a way to use find_package for libcurl
set(libcurl_DIR "${CMAKE_CURRENT_LIST_DIR}/curl")

list(APPEND CMAKE_PREFIX_PATH <${libcurl_DIR}/include>)
set(libcurl_INCLUDE_DIR "${libcurl_DIR}/include")
set(libcurl_LIBRARY "${libcurl_DIR}/lib/libcurl.dll.a")

include_directories(${libcurl_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(lib-examples)