cmake_minimum_required(VERSION 3.31)
project(4sem_practice)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
        abstract.cpp
        arrows.cpp
        base.cpp
        bikes.cpp
        bonuses.cpp
        main.cpp
        obstacles.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})