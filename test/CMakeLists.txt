cmake_minimum_required(VERSION 3.26)
set(CMAKE_TOOLCHAIN_FILE
        /home/sid/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake
)
project(protocol_tests)

set(CMAKE_CXX_STANDARD 20)

find_package(Protobuf REQUIRED)
set(protos
        src/proto/test.proto
        src/proto/addressbook.proto
)
add_library(${PROJECT_NAME}_lib ${protos})
protobuf_generate(
        TARGET ${PROJECT_NAME}_lib
        LANGUAGE cpp
        OUT_VAR PROTO_SRCS
        PROTOS "${protos}"
        PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/../gen/cpp"
        IMPORT_DIRS "${PROJECT_SOURCE_DIR}/src/proto/"
        APPEND_PATH
)


target_include_directories(${PROJECT_NAME}_lib PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJECT_SOURCE_DIR}/../gen/cpp)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${Protobuf_LIBRARIES})

add_executable(${PROJECT_NAME} src/main.cpp)

find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")

target_include_directories(${PROJECT_NAME} PRIVATE ${Protobuf_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../gen/cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${RAPIDXML_INCLUDE_DIRS})


target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PROJECT_NAME}_lib
)


