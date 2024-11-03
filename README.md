[![aksdev-blog](https://img.shields.io/badge/blog-akselmo.dev-blue?style=flat-square)](https://akselmo.dev)
[![mastodon](https://img.shields.io/mastodon/follow/109598121971865878?color=%233088D4&domain=https://tilde.zone&logo=mastodon&style=flat-square&logoColor=white)](https://tilde.zone/@aks)
[![itchio](https://img.shields.io/badge/itch.io-akselmo-%23FA5C5C?style=flat-square&logo=itch.io&logoColor=white)](https://akselmo.itch.io/)
[![ko-fi](https://img.shields.io/badge/ko--fi-donate-%23FF5E5B?style=flat-square&logo=ko-fi&logoColor=white)](https://ko-fi.com/L4L57FOPF)
[![Liberapay patrons](https://img.shields.io/liberapay/patrons/akselmo?label=LiberaPay&logo=liberapay&logoColor=ffffff&style=flat-square)](https://liberapay.com/akselmo/)


# Artificial Rage - a Scifi FPS 

<p align="center"><img src="./other_assets/ar_2.png" width="100"/> <a href="https://codeberg.org/akselmo/artificial-rage"><img src="https://akselmo.dev/assets/images/getitoncodeberg.svg" width="200"></a></p>
<p align="center"><img alt="Screenshot of Artificial Rage" src="./screenshot.png" /></p>


A simple FPS project (Wolfenstein 3D clone), made with [Raylib](https://www.raylib.com/) and [Odin](https://odin-lang.org/) programming language.

This project was originally a way for me to learn programming C, but it got rather tiresome to work with C..
So I moved the whole thing to Odin instead!

The Odin code is still somewhat messy since I basically "transpiled" all the C code to Odin. It will
be cleaned over time. Hopefully.

Here's a blog post I wrote about the ordeal: [https://akselmo.dev/posts/moving-from-c-to-odin/](https://akselmo.dev/posts/moving-from-c-to-odin/)

I also hope that one day this game will be fun to play, and I want
people to tinker with the source code, create mods, maps etc..

I also want this project to be a starting point for those who want to make their own simple FPS, or to be used
as a learning tool.

## Contributing

Currently not looking for contributions for code, but any other creative assets such as art is welcome!

If you find glaring bugs or issues in the code, I welcome issues and critique so I can fix them!

## How to compile for different platforms

First, get the dependencies that Raylib uses: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux

The game should run on anywhere where Raylib compiles.

### Using Odin to build

Since this project uses Odin, it's rather simple to build and run.

The Odin version I am using is usually latest git master, but when there's a 
stable versions in Fedora packages, I will likely stick with those.

```sh
# For building
odin build ./src -collection:src=src -debug -out:./build/ArtificialRage

# For building and running
odin run ./src -collection:src=src -debug -out:./build/ArtificialRage

# There's also corresponding ./build.sh and ./run.sh files for the above commands

```

Make sure to run the game in same folder where `assets` folder is. These assets are needed for level textures and so on.

### Debugging

You can use LLDB to debug. I use LLDB DAP with Kate and it works really well for this,
but any debugger (visual or CLI) that uses LLDB should work.

## TODO:

Check this project for TODO list: https://codeberg.org/akselmo/artificial-rage/projects/1358

## Old C code

The old C codebase can be found from this commit: [e93149492545cbc93971c07cc9e7a976ca5ff35e](https://codeberg.org/akselmo/artificial-rage/src/commit/e93149492545cbc93971c07cc9e7a976ca5ff35e).
