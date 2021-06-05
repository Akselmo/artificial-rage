# Artificial Rage - a Scifi FPS

WIP

## Howto compile for different platforms

WIP. These notes are just for myself for now.

* Compile raylib for your platform from source

* Copy your built libraylib.a into lib
    * Right now the Cmake file only supports building for windows
    * Plan is to make it support other systems too

* Copy the following to include:
    * raylib.h
    * raymath.h

Make sure CMake builds the game for your platform. Right now it's setup for windows only
