cmake_minimum_required(VERSION 3.10)
project(Xiaoming'sRobot)

set(CXX_STANDARD 17)

aux_source_directory(./src src)

add_definitions(-DDEBUG)

add_executable(main ${src})