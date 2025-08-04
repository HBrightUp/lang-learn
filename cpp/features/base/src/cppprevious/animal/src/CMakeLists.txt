project (animal)

add_library(${PROJECT_NAME} animal.cpp cat.cpp dog.cpp)


target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)