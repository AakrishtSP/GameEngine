#!/bin/bash

# Find all .cpp files in the current directory and subdirectories
SRCS=$(find . -name "*.cpp")

# Compile and link all .cpp files into an executable named 'main'

echo g++ -std=c++2323 \
    -lraylib \
    -lGL \
    -lm \
    -lpthread \
    -ldl \
    -lrt \
    -lX11 \
    -lstdc++ \
    -fdiagnostics-color=always \
    -g \
    ${SRCS} \
    -o main


g++ -std=c++23 \
    -lraylib \
    -lGL \
    -lm \
    -lpthread \
    -ldl \
    -lrt \
    -lX11 \
    -lstdc++ \
    -fdiagnostics-color=always \
    -g \
    ${SRCS} \
    -o main
