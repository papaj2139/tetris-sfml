# Tetris in SFML

A Tetris game I made using C++ and SFML,  
made for learning purposes to get into SFML and C++,  
and I can say it turned out pretty well.

## running

### windows:
1. download [SFML](https://www.sfml-dev.org/download.php).
2. set up sfml with you irde (visual studio, code::blocks, etc.).
3. make sure sfml is linked
4. build and run

### linux:
1. install sfml:  
   `sudo apt-get install libsfml-dev`
2. compile and run  
   `g++ -o tetris main.cpp -lsfml-graphics -lsfml-window -lsfml-system`  
   `./tetris`

### macos:
1. download sfml using homebrew:  
   `brew install sfml`
2. compile and run:  
   `g++ -o tetris main.cpp -lsfml-graphics -lsfml-window -lsfml-system`  
   `./tetris`


#TODO 
-music,sounds
-levels
-change font
