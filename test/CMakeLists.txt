cmake_minimum_required(VERSION 3.26)

if(NOT DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT /home/sid/.clion-vcpkg/vcpkg)
else()
    cmake_path(SET VCPKG_ROOT NORMALIZE $ENV{VCPKG_ROOT} )
endif()

set(CMAKE_TOOLCHAIN_FILE
        ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
)

message("CMAKE_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE}")

project(protocol_tests)

set(CMAKE_CXX_STANDARD 20)

set(protos
        src/proto/test.proto
)
add_library(${PROJECT_NAME}_lib ${protos})

find_package(Protobuf REQUIRED)

protobuf_generate(
        TARGET ${PROJECT_NAME}_lib
        LANGUAGE cpp
        OUT_VAR PROTO_SRCS
        PROTOS "${protos}"
        PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/../gen/cpp"
        IMPORT_DIRS
            "${PROJECT_SOURCE_DIR}/src/proto/"
            "${PROJECT_SOURCE_DIR}/../src/pb2xml"
        APPEND_PATH
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJECT_SOURCE_DIR}/../gen/cpp)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${Protobuf_LIBRARIES})



add_executable(${PROJECT_NAME} src/main.cpp)

find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${Protobuf_INCLUDE_DIRS}
        ${RAPIDXML_INCLUDE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/../gen/cpp
        ${CMAKE_CURRENT_LIST_DIR}/../src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        pb2xml
        ${PROJECT_NAME}_lib
)

