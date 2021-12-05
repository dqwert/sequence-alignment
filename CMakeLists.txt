cmake_minimum_required(VERSION 3.20)
project(sequence_alignment)

set(CMAKE_CXX_STANDARD 17)

add_executable(sequence_alignment main.cpp SequenceGen.cpp SequenceGen.h)
