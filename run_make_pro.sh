#!/bin/bash

# Generate header file
make generate_files

# Refresh c++ header files to include the new generated ones
./run_cmake.sh

# Build project
make bashc
