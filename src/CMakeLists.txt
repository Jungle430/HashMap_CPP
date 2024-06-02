cmake_minimum_required(VERSION 3.2)
include_directories(include)

file(COPY source/display.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/source)

set(SRC_SOURCE display.cpp)

add_library(HASH_MAP ${SRC_SOURCE})