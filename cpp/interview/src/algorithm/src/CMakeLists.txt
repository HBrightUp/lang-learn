
file(GLOB HEADERS "../include/*.h")

#add_subdirectory(animal/src)

add_library( algorithm 
    algorithm.cpp 
    sort.cpp
    ${HEADERS}
)

target_link_libraries(
    algorithm
    common
)

