[![aksdev-blog](https://img.shields.io/badge/blog-akselmo.dev-blue?style=flat-square)](https://akselmo.dev)
[![mastodon](https://img.shields.io/mastodon/follow/109598121971865878?color=%233088D4&domain=https://tilde.zone&logo=mastodon&style=flat-square&logoColor=white)](https://tilde.zone/@aks)
[![itchio](https://img.shields.io/badge/itch.io-akselmo-%23FA5C5C?style=flat-square&logo=itch.io&logoColor=white)](https://akselmo.itch.io/)
[![ko-fi](https://img.shields.io/badge/ko--fi-donate-%23FF5E5B?style=flat-square&logo=ko-fi&logoColor=white)](https://ko-fi.com/L4L57FOPF)
[![Liberapay patrons](https://img.shields.io/liberapay/patrons/akselmo?label=LiberaPay&logo=liberapay&logoColor=ffffff&style=flat-square)](https://liberapay.com/akselmo/)
[![Matrix](https://img.shields.io/matrix/aksdev-space:matrix.akselmo.dev?color=0dbd8b&label=aks_dev%20matrix&logo=matrix&logoColor=ffffff&server_fqdn=matrix.org&style=flat-square)](https://matrix.to/#/#aksdev-space:matrix.akselmo.dev)


# Artificial Rage - a Scifi FPS 

<p align="center"><img src="./other_assets/ar_2.png" width="100"/> <a href="https://codeberg.org/akselmo/artificial-rage"><img src="https://akselmo.dev/assets/images/getitoncodeberg.svg" width="200"></a></p>
<p align="center"><img alt="Screenshot of Artificial Rage" src="./screenshot.png" /></p>


A simple FPS project (Wolfenstein 3D clone), made with [Raylib](https://www.raylib.com/) and pure C.

This project is a learning platform for me to learn C. I also hope that one day this game will be fun to play, and I want
people to tinker with the source code, create mods, maps etc..

I also want this project to be a starting point for those who want to make their own simple FPS, or to be used
as a learning tool.

## Contributing

Currently not looking for contributions for code, but any other creative assets such as art is welcome!

If you find glaring bugs or issues in the code, I welcome issues and critique so I can fix them!

## How to compile for different platforms

First, get the dependencies that Raylib uses: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux

The game should run on anywhere where Raylib compiles.

### CMake

This project uses CMake to build.

Here's the basic commands you need.

```sh
# build
git clone https://codeberg.org/akselmo/artificial-rage.git
cd artificial-rage
cmake -Bbuild && cmake --build build --parallel
# run
cd build
./ArtificialRage
```

Make sure to run the game in same folder where `assets` folder is. These assets are needed for level textures and so on.

### XMake

If you wish to use XMake instead, the xmake.lua can be found from here: [xmake.lua](https://codeberg.org/akselmo/artificial-rage/src/commit/98f54fc2b42be833f1b1f79b3a4eb086533777e1/xmake.lua)

## TODO:
Check this project for TODO list: https://codeberg.org/akselmo/artificial-rage/projects/1358

