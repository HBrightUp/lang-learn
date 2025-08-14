cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508") 

set(CMAKE_BUILD_TYPE "Debug")

project(flying)

FILE(GLOB proto_files ${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto)  
FOREACH(proto_file ${proto_files})
    STRING(REGEX REPLACE "[^/]proto" "" proto_file_name ${proto_file})
    LIST(APPEND PROTO_SRCS "${proto_file_name}.pb.cc")
	ADD_CUSTOM_COMMAND(
		OUTPUT "${proto_file_name}.pb.h" "${proto_file_name}.pb.cc"
		COMMAND protoc --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto
                       --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/proto/ ${proto_file} 
		DEPENDS ${proto_file}
	)
ENDFOREACH(proto_file)
message(STATUS ${PROTO_SRCS})

include_directories(./proto)
add_executable(${PROJECT_NAME} main.cpp ${PROTO_SRCS}) 
target_link_libraries( ${PROJECT_NAME} protobuf)