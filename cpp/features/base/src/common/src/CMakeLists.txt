
project(common)

file(GLOB HEADERS "../include/*.h")

#add_subdirectory(animal/src)

add_library( ${PROJECT_NAME} 
    source_location.cpp 

    ${HEADERS}
)

#target_include_directories( ${PROJECT_NAME}
#    PUBLIC ${PROJECT_SOURCE_DIR}/include
#)

#target_link_libraries(${PROJECT_NAME} 
#    cpp23
#)