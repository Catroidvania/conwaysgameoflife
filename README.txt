CONWAYS GAME OF LIFE, BADLY

--------------------------------------------------------------------------------
COMPILING
--------------------------------------------------------------------------------
this program requires the ncurses library (installed by default on most systems)
and GNU make (mingw on windows)

compile with `make`
remove object with `make clean`
run the `main` executable

--------------------------------------------------------------------------------
RULES TO CONWAYS GAME OF LIFE
--------------------------------------------------------------------------------
- tiles are either living (shown as @) or dead (shown as .)
- a neighbor is a living surrounding tile
- living tiles with 2 or 3 neighbors survive
- living tiles with less than 2 neighbors die
- living tiles with more than 3 neighbors die
- dead tiles with exactly 3 neightbors live

see more about conways game of life on wikipedia:
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

--------------------------------------------------------------------------------
CONTROLS
--------------------------------------------------------------------------------
click - switch state of tile under cursor
q - quits the program
c - kill all tiles
r - randomise state of all tiles
+ or > - decrease game speed
- or < - increase game speed
0 to 9 - change game speed
space - pause game updates

enjoy!
