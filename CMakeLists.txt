cmake_minimum_required(VERSION 3.8)
project(finite_automaton)

set(CMAKE_CXX_STANDARD 20)

#---
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#---

include_directories(${PROJECT_SOURCE_DIR})

set(    GENERAL_SOURCE
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
        source/regular_expression/correct_string.cpp
        source/regular_expression/parse_reverse_polish_notation.cpp
        source/practice1/parser.cpp
        )

set(    MAIN_SOURCE
        main/main.cpp
        ${GENERAL_SOURCE}
        )

#---
set(EXTERNAL_INSTALL_LOCATION ${Testing_SOURCE_DIR}/3rdparty)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Threads REQUIRED)

set(TEST_SOURCES
        tests/tests.cpp
        tests/practice1.cpp
        ${GENERAL_SOURCE})

add_executable(test ${TEST_SOURCES})

target_link_libraries(test gtest_main gtest Threads::Threads)

#---

add_executable(finite_automaton ${MAIN_SOURCE})
