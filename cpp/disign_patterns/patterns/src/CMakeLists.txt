
file(GLOB HEADERS "../include/*.h" "../include/*.hpp")



add_library( patterns 
    examples.cpp

    ${HEADERS}
)

