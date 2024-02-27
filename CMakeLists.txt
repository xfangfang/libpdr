cmake_minimum_required(VERSION 3.15)
project(pdr)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_PDR_EXAMPLE "build pdr example" OFF)

include(FetchContent)
FetchContent_Declare(mongoose GIT_REPOSITORY https://github.com/cesanta/mongoose.git
        GIT_TAG 52997c6c5170050148d23436d2ed398811f48c30) # 7.13
FetchContent_MakeAvailable(mongoose)

FetchContent_Declare(tinyxml2 GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
        GIT_TAG e05956094c27117f989d22f25b75633123d72a83) # 2023/01/15
FetchContent_MakeAvailable(tinyxml2)

file(GLOB_RECURSE MAIN_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME}
        ${MAIN_SRC}
        ${CMAKE_BINARY_DIR}/_deps/mongoose-src/mongoose.c
        )
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/_deps/mongoose-src
        )
target_link_libraries(${PROJECT_NAME} PUBLIC tinyxml2)

if (BUILD_PDR_EXAMPLE)
    message(STATUS "Build pdr example: server")

    add_executable(libpdr_server example/server.cpp)
    target_link_libraries(libpdr_server ${PROJECT_NAME})
endif ()