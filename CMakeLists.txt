cmake_minimum_required(VERSION 3.16)
project(UnixFileSysStimulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(UnixFileSysStimulator main.cpp filesys.h try.cpp blockmanage.cpp utils.cpp igetput.cpp iallocfree.cpp format.cpp create.cpp)