# Artificial Rage - a Scifi FPS

<p>
    <a href="https://discord.gg/PZkYZRx">
    <img src="https://img.shields.io/discord/475097536160595979?label=Skale%20Games%20Discord"></a>
    <a href="https://matrix.to/#/#skalegames:matrix.org">
    <img src="https://img.shields.io/matrix/skalegames:matrix.org?label=Skale%20Games%20Matrix"></a>
</p>

A simple FPS project

## Howto compile for different platforms

WIP. These notes are just for myself for now.

* Compile raylib for your platform from source

* Copy your built libraylib.a into lib

* Copy the following to include:
  * raylib.h
  * raymath.h

**Remember to run "cmake configure" in vscode (or equivalent in any other system) before building, when adding new files.**

## Entity RGB Values

| Entity Name        | RGB Value   |
|--------------------|-------------|
| NONE               | 0,0,0       |
| wall               | 255,255,255 |
| levelStartPosition | 0,255,0     |
| levelEndPosition   | 0,0,255     |
| enemy              | 255,0,0     |
