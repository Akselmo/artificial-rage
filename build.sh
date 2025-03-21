#!/usr/bin/env bash

mkdir -p ./build
odin build ./src -collection:src=src -debug -out:./build/ArtificialRage
