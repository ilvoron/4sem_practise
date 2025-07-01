cmake_minimum_required(VERSION 3.31)
project(4sem_practice)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})