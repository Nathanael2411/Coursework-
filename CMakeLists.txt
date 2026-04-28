cmake_minimum_required(VERSION 3.31)
project(Power_quality_analyser C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled main.c
        waveform.c
        waveform.h
        io.c
        io.h)