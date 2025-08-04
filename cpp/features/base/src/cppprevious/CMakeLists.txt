project (cppprevious)

file(GLOB HEADERS "include/*.h")

add_subdirectory(animal/src)

add_library(cppprevious 
    interfaces.cpp 
    object.cpp

    ${HEADERS}
)

#target_include_directories( ${PROJECT_NAME}
#    PUBLIC ${PROJECT_SOURCE_DIR}/include
#)

target_link_libraries(${PROJECT_NAME} 
    animal
)