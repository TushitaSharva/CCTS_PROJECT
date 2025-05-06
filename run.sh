#!/bin/bash

# Clear the outputs directory if it exists
outputs_directory="./outputs"
if [ -d "$outputs_directory" ]; then
    rm -rf "$outputs_directory"/*
fi

# Input file
input_file_name="inp.txt"
g++ -std=c++17 O2PL.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp Scheduler.cpp -o o2pl.out
g++ -std=c++17 O-O2PL.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp OptimisticScheduler.cpp -o oo2pl.out
./o2pl.out "inp.txt"
./oo2pl.out "inp.txt"

# g++ -std=c++17 O2PL.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp Scheduler.cpp -o o2pl.out
# g++ -std=c++17 O-O2PL.cpp NodeList.cpp DataItem.cpp WaitsForGraph.cpp OptimisticScheduler.cpp -o oo2pl.out
# for input_file_name in ./inputs/E2/*.txt; do
#     echo "Experiment: $input_file_name"
#     ./a.out "$input_file_name"
#     echo ""

#     outputs_directory="./outputs"
#     if [ -d "$outputs_directory" ]; then
#         rm -rf "$outputs_directory"/*
#     fi
# done

# for input_file_name in ./inputs/E3/*.txt; do
#     echo "Experiment: $input_file_name"
#     ./a.out "$input_file_name"
#     echo ""

#     outputs_directory="./outputs"
#     if [ -d "$outputs_directory" ]; then
#         rm -rf "$outputs_directory"/*
#     fi
# done