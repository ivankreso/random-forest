cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to RELEASE")
  set(CMAKE_BUILD_TYPE "release")
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
message("Mode: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -std=c++11 -march=native")
elseif(CMAKE_BUILD_TYPE MATCHES RELEASE)
  #set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -std=c++11 -march=native -fopenmp")
  set(CMAKE_CXX_FLAGS "-pedantic -Wall -std=c++11 -march=native -fopenmp")
else()
  message(FATAL_ERROR "Unknwon Mode: ${CMAKE_BUILD_TYPE}")
endif()

file(GLOB SRC_LIST *.cc ../../*.cc)

add_subdirectory(../../random_forest/ libs/random_forest)

message(STATUS "${SRC_LIST}")

add_executable(test_uci ${SRC_LIST})
target_link_libraries(test_uci random_forest)

