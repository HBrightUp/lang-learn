
file(GLOB HEADERS "../include/*.h")

add_library( cpp20 
    interfaces.cpp 
    concept.cpp
    ${HEADERS}
)

