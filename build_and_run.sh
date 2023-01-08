#!/usr/bin/env bash

xmake project -k compile_commands
xmake -b -r
xmake run