
file(GLOB HEADERS "../include/*.h" "../include/*.hpp")

#add_subdirectory(animal/src)

add_library( common 
    generate.cpp

    ${HEADERS}
)

