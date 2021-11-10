# Artificial Rage - a Scifi FPS

<p>
    <a href="https://discord.gg/PZkYZRx">
    <img src="https://img.shields.io/discord/475097536160595979?label=Skale%20Games%20Discord"></a>
    <a href="https://matrix.to/#/#skalegames:matrix.org">
    <img src="https://img.shields.io/matrix/skalegames:matrix.org?label=Skale%20Games%20Matrix"></a>
</p>

A simple FPS project (Wolfenstein 3D clone), made with [Raylib](https://www.raylib.com/) and pure C.

This project is a learning platform for me to learn C. I also hope one day this game will be fun to play, 
and I want people to tinker with the source code, create mods, maps etc..

I also want this project to be a starting point for some who want to make their own simple FPS, 
or they could use this as a learning tool.

If you want to play my "bigger" games: https://akselmo.itch.io/

## Howto compile for different platforms

WIP. These notes are just for myself for now.

* Compile raylib for your platform from source

* Copy your built libraylib.a into lib

* Copy the following to include:
  * raylib.h
  * raymath.h

**Remember to run "cmake configure" in vscode (or equivalent in any other system) before building, when adding new files.**

## TODO:

A very quick and dirty TODO list

MVP features:
- [x] custom player camera
- [x] level generation
- [x] collisions for enemy/player/wall
- [ ] weapon system
- [ ] enemy movement
- [ ] enemy shoot player
    - [ ] player loses health
- [ ] player shoot enemy
    - [ ] enemy loses health
    - [ ] enemy death
- [ ] items
    - [ ] pick up weapon, adds weapon and ammo
    - [ ] pick up health
    - [ ] pick up id-card to make ending teleporter work
- [ ] menu (not needed for MVP really)
    - [ ] main menu that has start, quit, options that tell about options file
    - [ ] level loading screen
        - [ ] one folder contains numbered folders that contain map files
    - [ ] pause menu that has continue, restart and quit
    - [ ] death screen that is pause menu with different title
- [ ] ui
    - [ ] player health
    - [ ] player active weapon
        - [ ] ammo

Nice to haves:

- [ ] clutter?
    - [ ] some clutter items you can walk through, they dont block
    - [ ] spawned in random positions on map?
- [ ] doors?
    - [ ] doors could be just walls that can be activated
    - [ ] when activated they move down, then slide up at some point if player is not on same grid

Code specific:

- [ ] clean up
- [ ] make more helper functions instead of cross referencing?
- [ ] PROTOTYPES!!!!

Art:

- [ ] Weapon sprites
- [ ] Enemy sprites
- [ ] Some HUD stuff
- [ ] UI buttons etc?
- [ ] level stuff

Sound/Music:

- [ ] Simple gameplay music loops
- [ ] weapon sounds
- [ ] player sounds
- [ ] enemy sounds


## Entity RGB Values

| Entity Name        | RGB Value   |
|--------------------|-------------|
| NONE               | 0,0,0       |
| wall               | 255,255,255 |
| levelStartPosition | 0,255,0     |
| levelEndPosition   | 0,0,255     |
| enemy              | 255,0,0     |
