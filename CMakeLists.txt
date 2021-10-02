cmake_minimum_required(VERSION 3.20)
project(finite_automaton)

set(CMAKE_CXX_STANDARD 20)

#---
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#---

include_directories(${PROJECT_SOURCE_DIR})

set(    MAIN_SOURCE
        main/main.cpp
        source/regular_expression/transformer.cpp
        source/sub_automaton.cpp
        source/finite_automaton.cpp

    )

add_executable(finite_automaton ${MAIN_SOURCE})