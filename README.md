[![aksdev-blog](https://img.shields.io/badge/blog-akselmo.dev-blue?style=flat-square)](https://akselmo.dev)
[![mastodon](https://img.shields.io/mastodon/follow/106864208846697693?color=%233088D4&domain=https%3A%2F%2Fmastodon.technology&logo=mastodon&style=flat-square&logoColor=white)](https://mastodon.technology/@huntra)
[![itchio](https://img.shields.io/badge/itch.io-akselmo-%23FA5C5C?style=flat-square&logo=itch.io&logoColor=white)](https://akselmo.itch.io/)
[![ko-fi](https://img.shields.io/badge/ko--fi-donate-%23FF5E5B?style=flat-square&logo=ko-fi&logoColor=white)](https://ko-fi.com/L4L57FOPF)
[![discord](https://img.shields.io/discord/475097536160595979?color=%235865F2&label=aks_dev%20discord&logo=discord&style=flat-square&logoColor=white)](https://discord.gg/PZkYZRx)
# Artificial Rage - a Scifi FPS

-------

A simple FPS project (Wolfenstein 3D clone), made with [Raylib](https://www.raylib.com/) and pure C.

This project is a learning platform for me to learn C. I also hope one day this game will be fun to play, and I want
people to tinker with the source code, create mods, maps etc..

I also want this project to be a starting point for some who want to make their own simple FPS, or they could use this
as a learning tool.


## Howto compile for different platforms

WIP. These notes are just for myself for now.

* Compile raylib for your platform from source

* Copy your built libraylib.a into lib

* Copy the following to include:
    * raylib.h
    * raymath.h

* Run the following commands in project root to build the binary
    * `mkdir build && cd build`
    * `cmake ../`
    * `make`

**Remember to run "cmake configure" in vscode (or equivalent in any other system) before building, when adding new
files.**

## TODO:
Check this project for TODO list: https://github.com/users/Akselmo/projects/1/views/1


