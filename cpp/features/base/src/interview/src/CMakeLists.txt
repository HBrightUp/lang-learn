
file(GLOB HEADERS "../include/*.h")

add_library( interview 
    interfaces.cpp 
    ${HEADERS}
)

