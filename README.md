[![aksdev-blog](https://img.shields.io/badge/blog-akselmo.dev-blue?style=flat-square)](https://akselmo.dev)
[![mastodon](https://img.shields.io/mastodon/follow/106864208846697693?color=%233088D4&domain=https%3A%2F%2Fmastodon.technology&logo=mastodon&style=flat-square&logoColor=white)](https://mastodon.technology/@huntra)
[![itchio](https://img.shields.io/badge/itch.io-akselmo-%23FA5C5C?style=flat-square&logo=itch.io&logoColor=white)](https://akselmo.itch.io/)
[![ko-fi](https://img.shields.io/badge/ko--fi-donate-%23FF5E5B?style=flat-square&logo=ko-fi&logoColor=white)](https://ko-fi.com/L4L57FOPF)
[![Liberapay patrons](https://img.shields.io/liberapay/patrons/akselmo?label=LiberaPay&logo=liberapay&logoColor=ffffff&style=flat-square)](https://liberapay.com/akselmo/)
[![discord](https://img.shields.io/discord/475097536160595979?color=%235865F2&label=aks_dev%20discord&logo=discord&style=flat-square&logoColor=white)](https://discord.gg/PZkYZRx)
[![Matrix](https://img.shields.io/matrix/aksdev-space:matrix.akselmo.dev?color=0dbd8b&label=aks_dev%20matrix&logo=matrix&logoColor=ffffff&server_fqdn=matrix.org&style=flat-square)](https://matrix.to/#/#aksdev-space:matrix.akselmo.dev)


# Artificial Rage - a Scifi FPS 

<p align="center"><img src="./other_assets/ar_2.png" width="100"/> <a href="https://codeberg.org/akselmo/artificial-rage"><img src="https://akselmo.dev/assets/images/getitoncodeberg.svg" width="200"></a></p>
<p align="center"><img alt="Screenshot of Artificial Rage" src="./screenshot.png" /></p>


A simple FPS project (Wolfenstein 3D clone), made with [Raylib](https://www.raylib.com/) and pure C.

This project is a learning platform for me to learn C. I also hope one day this game will be fun to play, and I want
people to tinker with the source code, create mods, maps etc..

I also want this project to be a starting point for some who want to make their own simple FPS, or they could use this
as a learning tool.

## Contributing

Currently not looking for contributions for code, but any other creative assets such as art is welcome!

If you find glaring bugs or issues in the code, I welcome issues and critique so I can fix them!

## Howto compile for different platforms

First, get same dependencies Raylib uses: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux

This project uses [xmake](https://xmake.io/) build system. It is quite simple to use and should 
work for anyone.

* After installing xmake, just run `xmake` in the project root directory.
* If you want to use xmake to run the project, just type `xmake run`.
* For debugging with GDB for example, you can type `xmake run -d`.
* If you need CMake files, for example for an IDE that doesn't work with xmake, run `xmake project -k cmake -y` to generate `CMakeLists.txt` file

There are also extensions for various editors and IDEs for xmake.

If you don't want to use xmake, the older commits should have cmake configs lying around.
But xmake is highly recommended!

## TODO:
Check this project for TODO list: https://codeberg.org/akselmo/artificial-rage/projects/1358

