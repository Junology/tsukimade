cmake_minimum_required(VERSION 3.3.0)
# It seems that FindDoxygen requires v3.3.0 or later

## Project setting
project(tsukimade)

set(PROJECT_LUA lua5.2)
set(PROJECT_VERSION 0.2)
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_TEST_DIR ${CMAKE_SOURCE_DIR}/test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

## Language setting
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_language(C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


## Add directories
include_directories(${PROJECT_INCLUDE_DIR})

add_subdirectory(test)
add_subdirectory(doc)
