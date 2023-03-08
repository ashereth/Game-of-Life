# Assignment 4: The Game of Life
Name: Asher Etherington\
Cruz ID: aetherin\
Class: CSE13s

## Description:
This program runs a 'Game of Life' simulation on a grid/matrix. The 'Game of Life' was originally created\
by the English Mathematician John Horton Conway. The game is supposed to simulate how much life can evolve\
and change over time. The game works with a grid of squares in which each square, or cell, interacts with \
its neighbors throughout each iteration in a way defined by a simple set of rules. The rules for each iteration\
are;any live cell with two or three live neighbors survives, any dead cell with exactly 3 live neighbors becomes a\
live cell, and all other cells die. This game only takes input in the beginning and after that runs completely on its\
own until he set number of iterations is complete.

## Build:
To build this program simply download all files and enter that directory within a terminal and use the $ make or \
$ make all command. This will create the 'life' executable.

## Run:
Once you have made the 'life' executable it can be run using, $ ./life 'argument(s)'. In order to see the available\
arguments, simply run that file without any arguments.

## Cleaning:
To clean the directory of all executable run the command $ make clean.

## Available Command Line Arguments

• -t : Specify that the Game of Life is to be played on a toroidal universe.\
• -s : Silence ncurses. Enabling this option means that nothing will be displayed by ncurses.\
• -n generations : Specify the number of generations that the universe goes through. The default number of generations is 100.\
• -i input : Specify the input file to read in order to populate the universe. By default the input is stdin.\
• -o output : Specify the output file to print the final state of the universe to. By default the output is stdout.

