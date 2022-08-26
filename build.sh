#!/bin/sh
echo "Making build dir"
mkdir -p ./build
echo "Going to build dir"
cd ./build
echo "Running cmake"
cmake ../ -DCMAKE_BUILD_TYPE=Debug
echo "Building with cmake"
cmake --build . --config Debug --target ArtificialRage --
