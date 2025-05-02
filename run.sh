#!/bin/bash

# Clear the outputs directory if it exists
outputs_directory="./outputs"
if [ -d "$outputs_directory" ]; then
    rm -rf "$outputs_directory"/*
fi

# Input file
input_file_name="inp.txt"
g++ -std=c++17 main.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp Scheduler.cpp -o a.out
valgrind ./a.out "inp.txt"