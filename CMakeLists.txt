cmake_minimum_required(VERSION 3.29)
project(cs6740 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fno-stack-protector -z execstack")

add_executable(cs6740 main.c
        simple_crypto/simple_crypto.c
        employee_directory/employee_directory.c
        crypto_cracker/crypto_cracker.c)

