cmake_minimum_required(VERSION 3.20)
project(sequence_alignment)

set(CMAKE_CXX_STANDARD 17)

add_executable(sequence_alignment main.cpp sequence_utils.cpp sequence_utils.h dynamic_programming_algorithm.cpp dynamic_programming_algorithm.h)
add_executable(dynamic_programming dynamic_programming.cpp SequenceAlignment.cpp SequenceAlignment.h)
