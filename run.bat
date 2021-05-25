gcc main.c player.c settings.c level.c -o game.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include\ -L lib -lraylib -lopengl32 -lgdi32 -lwinmm

game.exe