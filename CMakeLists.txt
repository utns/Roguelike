project(Game)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
#SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -std=c++11")
#SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
add_executable(${PROJECT_NAME} main.cpp map.cpp characters.cpp controller.cpp)
