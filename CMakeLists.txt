cmake_minimum_required(VERSION 3.15)
project(libpdr)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_PDR_EXAMPLE "build pdr example" OFF)

file(GLOB_RECURSE MAIN_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME}
        ${MAIN_SRC}
        ${CMAKE_SOURCE_DIR}/library/mongoose/mongoose.c
        ${CMAKE_SOURCE_DIR}/library/tinyxml2/tinyxml2.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/library/mongoose
        ${CMAKE_SOURCE_DIR}/library/tinyxml2)

if (BUILD_PDR_EXAMPLE)
    message(STATUS "Build pdr example: server")

    add_executable(libpdr_server example/server.cpp)
    target_link_libraries(libpdr_server ${PROJECT_NAME})
endif ()