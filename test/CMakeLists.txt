project(test)
cmake_minimum_required(VERSION 3.10)
include_directories(..)

add_executable(${PROJECT_NAME} main.c ../bytearray.c)
