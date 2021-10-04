cmake_minimum_required(VERSION 3.16.3)
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
        source/automatons/sub_automaton.cpp
        source/automatons/finite.cpp
        source/empty_word_deleter/empty_word_deleter.cpp
        source/deterministic_automaton/transformer.cpp
        source/deterministic_automaton/delete_long_words.cpp
        source/automatons/deterministic.cpp
        source/automatons/no_empty_words.cpp source/automatons/no_empty_words.cpp
        source/automatons/complete_deterministic.cpp
        source/user_interface.cpp
        source/regular_expression/correct_string.cpp)

add_executable(finite_automaton ${MAIN_SOURCE})
