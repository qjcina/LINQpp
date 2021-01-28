cmake_minimum_required(VERSION 3.5)

set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_example ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})