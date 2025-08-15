
file(GLOB HEADERS "../include/*.h" "../include/*.hpp" )

add_library( syntax 
    bind.cpp 
    syntax.cpp
    type_traits.cpp
    source_location.cpp
    ${HEADERS}
)

