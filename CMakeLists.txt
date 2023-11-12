cmake_minimum_required(VERSION 3.26)
set(CMAKE_TOOLCHAIN_FILE
        /home/sid/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake
)
project(pb2xml)

set(CMAKE_CXX_STANDARD 20)


#set(protos src/proto/test.proto)
add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
        PRIVATE
        src/pb2xml/xml_format.cc
        PUBLIC FILE_SET ${PROJECT_NAME}_HEADERS
        TYPE HEADERS
        FILES
            src/pb2xml/xml_format.h
            src/pb2xml/rapidxml_ext.h
)

find_package(Protobuf REQUIRED)
find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")

target_include_directories(${PROJECT_NAME} PUBLIC src/)

#protobuf_generate(
#        TARGET pb2xml
#        LANGUAGE cpp
#        OUT_VAR PROTO_SRCS
#        PROTOS "${protos}"
#        PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/gen/cpp"
#        IMPORT_DIRS "${PROJECT_SOURCE_DIR}/src/proto/"
#        APPEND_PATH
#)

target_include_directories(${PROJECT_NAME} PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${RAPIDXML_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)


target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES})

add_subdirectory(test)
