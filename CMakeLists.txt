cmake_minimum_required(VERSION 3.5)

project(Linqplusplus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/external)

include_directories(src)

add_subdirectory (src) 
add_subdirectory (test)
add_subdirectory (${EXTERNAL_FOLDER}/googletest)
