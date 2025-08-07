
file(GLOB HEADERS "../include/*.h")

#add_subdirectory(animal/src)

add_library( example 

    example.cpp 
    int_array.cpp
    ${HEADERS}
)

