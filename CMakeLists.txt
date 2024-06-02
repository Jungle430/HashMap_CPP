cmake_minimum_required(VERSION 3.2)
project(HASHMAP_CPP VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER $ENV{clang})


set(THIRD_LIBRARY
    fmt::fmt
    gtest
    gtest_main
)

include_directories(${CMAKE_SOURCE_DIR}/fmt/include)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/fmt)
add_subdirectory(${CMAKE_SOURCE_DIR}/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(HASHMAP_CPP main.cpp)
target_link_libraries(HASHMAP_CPP HASH_MAP ${THIRD_LIBRARY})

enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/src/tests)