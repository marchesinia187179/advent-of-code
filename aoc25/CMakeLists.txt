cmake_minimum_required(VERSION 4.0)
project(aof25 C)

set(CMAKE_C_STANDARD 11)

add_executable(aof25 main.c
        day1/day_one.c
        day1/day_one.h
        day2/day_two.c
        day2/day_two.h
        day3/day_three.c
        day3/day_three.h)
