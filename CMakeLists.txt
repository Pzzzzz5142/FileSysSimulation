cmake_minimum_required(VERSION 3.16)
project(UnixFileSysStimulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(UnixFileSysStimulator main.cpp filesys.h try.cpp blockmanage.cpp utils.cpp igetput.cpp iallocfree.cpp format.cpp create.cpp name.cpp opfl.cpp rdwt.cpp close.cpp log.cpp install.cpp dir.cpp delete.cpp)