#!/bin/sh
echo "Making build dir"
mkdir -p ./build
echo "Going to build dir"
cd build
echo "Running cmake"
cmake ../
echo "Running make"
make
