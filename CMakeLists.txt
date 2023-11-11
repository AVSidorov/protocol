cmake_minimum_required(VERSION 3.26)
set(CMAKE_TOOLCHAIN_FILE
        /home/sid/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake
)
project(protocol)

set(CMAKE_CXX_STANDARD 20)

find_package(Protobuf REQUIRED)
#set(protos src/proto/test.proto)
#add_library(${PROJECT_NAME} ${protos})
#protobuf_generate(
#        TARGET ${PROJECT_NAME}
#        LANGUAGE cpp
#        OUT_VAR PROTO_SRCS
#        PROTOC_OUT_DIR ${PROJECT_SOURCE_DIR}
#)
#
#
#target_include_directories(${PROJECT_NAME} PUBLIC ${Protobuf_INCLUDE_DIRS})
#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
#
#target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES})

add_subdirectory(test)

