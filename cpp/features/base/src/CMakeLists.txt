project(flying)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508") 

add_subdirectory(interview/src)
add_subdirectory(cppprevious)
add_subdirectory(cpp20/src)
add_subdirectory(cpp23/src)
add_subdirectory(common/src)



add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} 
    cppprevious
    cpp20
    cpp23
    common
    interview
)