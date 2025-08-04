
file(GLOB HEADERS "../include/*.h")

#add_subdirectory(animal/src)

add_library( cpp23 
    interfaces.cpp 
    enum_class.cpp
    ${HEADERS}
)

#target_include_directories( ${PROJECT_NAME}
#    PUBLIC ${PROJECT_SOURCE_DIR}/include
#)

#target_link_libraries( cpp23 
   # common
#)