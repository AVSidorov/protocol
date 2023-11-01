cmake_minimum_required(VERSION 3.26)
set(CMAKE_TOOLCHAIN_FILE
        /home/sid/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake
)
project(protocol)

set(CMAKE_CXX_STANDARD 20)

find_package(Protobuf REQUIRED)
set(protos src/proto/test.proto)
add_library(${PROJECT_NAME} ${protos})
protobuf_generate(
        TARGET ${PROJECT_NAME}
        LANGUAGE cpp
        OUT_VAR PROTO_SRCS
        PROTOC_OUT_DIR ${PROJECT_SOURCE_DIR}
)


target_include_directories(${PROJECT_NAME} PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES})

find_package(nlohmann_json CONFIG REQUIRED)
add_executable(protocol_test main.cpp)
target_include_directories(protocol_test PRIVATE ${Protobuf_INCLUDE_DIRS})
target_include_directories(protocol_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(protocol_test PRIVATE json2xml/include)

target_link_libraries(protocol_test PRIVATE
        ${PROJECT_NAME}
        nlohmann_json::nlohmann_json
)


