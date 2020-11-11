cmake_minimum_required (VERSION 3.10.2)
project (lz78_test)

# add_definitions(-std=c++17)
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/obj)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(TEST_LIST main.cpp)
add_executable(lz78_test ${TEST_LIST})
