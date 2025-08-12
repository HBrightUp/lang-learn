
file(GLOB HEADERS "../include/*.h" "../include/*.hpp" )

add_library( thread 
    thread.cpp 
    convert.cpp
    pointer.cpp
    ${HEADERS}
)

