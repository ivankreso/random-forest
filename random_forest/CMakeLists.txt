cmake_minimum_required(VERSION 2.8)

file(GLOB SRC_LIST . *.cc)
add_library(random_forest ${SRC_LIST})

#target_link_libraries(random_forest opencv_core)
