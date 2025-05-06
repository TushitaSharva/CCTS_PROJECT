#!/bin/bash

# Clear the outputs directory if it exists
outputs_directory="./outputs"
if [ -d "$outputs_directory" ]; then
    rm -rf "$outputs_directory"/*
fi

# Input file
# input_file_name="inp.txt"
# g++ -std=c++17 O2PL.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp Scheduler.cpp -o a.out
# g++ -std=c++17 O-O2PL.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp OptimisticScheduler.cpp -o a.out
# valgrind ./a.out "inp.txt"

# g++ -std=c++17 O2PL.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp Scheduler.cpp -o a.out
g++ -std=c++17 O-O2PL-2.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp OptScheduler.cpp -o a.out
# Uncomment lines 37-47 to run loops
for input_file_name in ./inputs/*/*.txt; do
    echo "Experiment: $input_file_name"
    ./a.out "$input_file_name"
    echo ""

    outputs_directory="./outputs"
    if [ -d "$outputs_directory" ]; then
        rm -rf "$outputs_directory"/*
    fi
done