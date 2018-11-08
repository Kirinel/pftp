cmake_minimum_required(VERSION 3.10)
project(pftp C)

set(CMAKE_C_STANDARD 99)

add_executable(pftp main.c analyze.c analyze.h connect.c connect.h transfer.c transfer.h)