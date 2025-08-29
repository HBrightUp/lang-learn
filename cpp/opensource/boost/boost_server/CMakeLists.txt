set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATICLIB")

aux_source_directory(. SRC_LIST)

file(GLOB HEADERS "*.h")

add_library(txjoinserver ${SRC_LIST} ${HEADERS})

target_include_directories(txjoinserver PRIVATE ..)
target_link_libraries(txjoinserver PRIVATE  Boost::Thread devcore)
eth_use(txjoinserver OPTIONAL OpenSSL)
