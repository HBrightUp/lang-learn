
file(GLOB HEADERS "../include/*.h")

#add_subdirectory(animal/src)

add_library( algorithm 

    algorithm.cpp 
    ${HEADERS}
)

