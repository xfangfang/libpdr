cmake_minimum_required(VERSION 3.15)
project(pdr)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_PDR_EXAMPLE "build pdr example" OFF)

include(FetchContent)
FetchContent_Declare(mongoose GIT_REPOSITORY https://github.com/cesanta/mongoose.git
        GIT_TAG 7.14)
FetchContent_MakeAvailable(mongoose)

FetchContent_Declare(tinyxml2 GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
        GIT_TAG 10.0.0)
FetchContent_MakeAvailable(tinyxml2)

file(GLOB_RECURSE MAIN_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME}
        ${MAIN_SRC}
        ${mongoose_SOURCE_DIR}/mongoose.c
        )
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${mongoose_SOURCE_DIR}
        )
target_link_libraries(${PROJECT_NAME} PUBLIC tinyxml2)

if (BUILD_PDR_EXAMPLE)
    message(STATUS "Build pdr example: server")

    add_executable(libpdr_server example/server.cpp)
    target_link_libraries(libpdr_server ${PROJECT_NAME})
endif ()