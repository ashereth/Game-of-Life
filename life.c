#include "universe.c"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define DELAY 50000
char input[100]; //input string for file
char output[100]; //output string for file
#define OPTIONS "htsn:i:o:"

bool toroidal = false; //set toroidal to false
int generations = 100; //set default generations to 100
bool curser = false; //set curser to false
int main(int argc, char **argv) {
    int casei = 0; //counter for case i
    int caseo = 0; //counter for case o
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't':
            toroidal = true; //set toroidal to true
            break;
        case 's':
            curser = true; //turn off ncurses
            break;
        case 'n':
            generations = atoi(optarg); //change generations to integer value of optarg
            break;
        case 'i':
            ++casei;
            strcpy(input, optarg); //set input file
            break;
        case 'o':
            strcpy(output, optarg); //set ouput file
            ++caseo;
            break;
        case 'h':
            printf("SYNOPSIS\n\tConway's Game of Life\n\nUsage\n\t./life tsn:i:o:h\n\n");
            printf("OPTIONS\n");
            printf("\t-t:\t\tcreate your universe as toroidal\n");
            printf("\t-s:\t\tSilence the evolution aniation\n");
            printf("\t-n {number}:\tset the number of generations [default: 100]\n");
            printf("\t-i {file}:\tset input file [default: stdin]\n");
            printf("\t-o {file}:\tset output file [default: stdout]\n");
            return 0;
        }
    }
    if (!casei) {
        fscanf(stdin, "%s", input); //if no input file then call stdin
    }
    if (generations < 1) { //print error if theres not enough generations
        printf("invalid number of generations.\n");
        return 0;
    }
    FILE *infile;
    infile = fopen(input, "r");
    uint32_t newcols = 0;
    uint32_t newrows = 0;
    fscanf(infile, "%u %u\n", &newrows, &newcols);
    Universe *matrix = uv_create(
        newrows, newcols, toroidal); //once we have the column and row make that cell alive
    Universe *matrixB = uv_create(newrows, newcols, toroidal);
    //universe is created now populate it
    uv_populate(matrix, infile);
    //loop for every iteration
    int gen = 0;
    while (gen < generations) {
        ++gen; //increment number of generations
        if (!curser) { //if curser then show all iterations
            initscr(); //initialize the screen
            curs_set(FALSE); //hide curser
            clear(); //clear the window
            int rows = uv_rows(matrix);
            int columns = uv_cols(matrix);
            for (int r = 0; r < rows; ++r) { //iterate through all rows and columns
                for (int c = 0; c < columns; ++c) {
                    if (uv_get_cell(matrix, r, c)) {
                        mvprintw(r, c, "o");
                    } else {
                        mvprintw(r, c, "");
                    }
                }
            }
            refresh(); //refresh screen
            usleep(DELAY);
            endwin(); //close screen
        }

        //universe is made, time to check each cell for number of neighbors
        for (uint32_t i = 0; i < uv_rows(matrix); ++i) { //iterate through rows
            for (uint32_t j = 0; j < uv_cols(matrix); ++j) { //iterate through cols of each row
                if (uv_census(matrix, i, j) == 3) { //if it has 3 neighbors make it alive
                    uv_live_cell(matrixB, i, j);
                }
                //if it has 2 or 3 live neighbors and it's alive then it stays alive
                else if (((uv_census(matrix, i, j) == 2) || (uv_census(matrix, i, j) == 3))
                         && uv_get_cell(matrix, i, j)) {
                    uv_live_cell(matrixB, i, j);
                } else {
                    uv_dead_cell(matrixB, i, j);
                }
            }
        }
        Universe *temp = matrix; //swap the matrices before going again
        matrix = matrixB;
        matrixB = temp;
    }
    fclose(infile);
    //print final matrix into file outfile
    if (!caseo) {
        uv_print(matrix, stdout); //if no output file just print to terminal
    } else {
        FILE *outfile;
        outfile = fopen(output, "w");
        uv_print(matrix, outfile);
        fclose(outfile);
    }
    uv_delete(matrix); //clear all the memory for the universes
    uv_delete(matrixB);
    return 0;
}
