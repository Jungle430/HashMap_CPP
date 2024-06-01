cmake_minimum_required(VERSION 3.2)
project(HASHMAP_CPP VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER $ENV{clang})

include_directories(${CMAKE_SOURCE_DIR}/fmt/include)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

add_subdirectory(src)
add_subdirectory(${CMAKE_SOURCE_DIR}/fmt)

add_executable(HASHMAP_CPP main.cpp)
target_link_libraries(HASHMAP_CPP SRC)
target_link_libraries(HASHMAP_CPP fmt::fmt)