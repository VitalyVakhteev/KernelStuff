cmake_minimum_required(VERSION 3.30)
project(basickernelproject)

set(CMAKE_CXX_STANDARD 20)

add_executable(basickernelproject kernel_code/kernel.c
        64bitkernel/main.c)
