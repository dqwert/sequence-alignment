cmake_minimum_required(VERSION 3.20)
project(sequence_alignment)

set(CMAKE_CXX_STANDARD 17)

add_executable(dynamic_programming dynamic_programming.cpp SequenceAlignment.cpp SequenceAlignment.h)
