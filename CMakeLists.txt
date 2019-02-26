cmake_minimum_required(VERSION 3.0)
project(example)
add_executable(example
    example.cpp
    Exception.h
    Socket.cpp Socket.h
    ServerSocket.cpp ServerSocket.h
    ClientSocket.cpp ClientSocket.h)

target_compile_options(example PRIVATE -g)
