cmake_minimum_required(VERSION 3.29)
project(cs6740 C)

set(CMAKE_C_STANDARD 11)

add_executable(cs6740 main.c
        cmake-build-debug/simple_crypto.c)
