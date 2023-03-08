#include "universe.h"

#include <stdlib.h>
#include <string.h>

struct Universe /*set up the struct*/
{
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    /*if rows or columns arent more than 0 break*/
    /*if (rows <= 0 || cols <= 0){}*/
    bool **grid = malloc(rows * sizeof(bool *)); /*create an array of length = rows*/
    for (uint32_t i = 0; i < rows; ++i) {
        /*for each row add a row of size = cols*/
        grid[i] = malloc(cols * sizeof(bool));
    }
    Universe *u
        = (Universe *) calloc(1, sizeof(Universe)); /*initialize the universe with given parts*/
    u->rows = rows;
    u->cols = cols;
    u->grid = grid;
    u->toroidal = toroidal;
    return u;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < uv_rows(u); i++) { /*iterate through each row freeing the rows*/
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    free(u->grid); /*once the content of the rows is freed, free the whole grid*/
    u->grid = NULL;
    free(u);
    return;
}

uint32_t uv_rows(Universe *u) {
    /*returns the number of rows*/
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    /*return the number of columns*/
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = 1;
    return;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = 0;
    return;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (u->grid[r][c]) {
        return true;
    }
    return false;
}

bool uv_populate(Universe *u, FILE *infile) { //may need to reduce column numbers in final
    if (infile == NULL) {
        printf("error");
        return false;
    }
    uint32_t new, newcol, newrow;
    int column = 0; //counter to see what column of file we are on
    while (fscanf(infile, "%u", &new) != EOF) { //get contents of file until at EOF
        ++column; //iterate counter
        if (column == 1) { //start doing stuff at 3 because we want to skip the first line of file
            if (new > u->rows) { //make sure the row is in bounds
                printf("row is out of bounds\n");
                return false;
            }
            newrow = new;
        }
        if (column == 2) {
            if (new > u->cols) { //make sure the column is in bounds
                printf("column is out of bounds\n");
                return false;
            }
            newcol = new;
            uv_live_cell(u, newrow, newcol); //once we have the column and row make that cell alive
            column -= 2; //reset column counter
        }
    }
    return true;
}
//got code for this function from TA John Yu
uint32_t isNeighbor(Universe *u, uint32_t r, uint32_t c) {
    if (r > 0 && r <= u->rows - 1 && c <= u->cols - 1) {
        return u->grid[r][c];
    }
    return 0;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t neighbors = 0;
    if (u->toroidal) {
        return neighbors;
    }
    //code for non toroidal universe from TA John Yu
    else {
        for (int row = -1; row <= 1; row++) {
            for (int col = -1; col <= 1; col++) {
                if (row != 0 || col != 0) {
                    neighbors += isNeighbor(u, r + row, c + col);
                }
            }
        }
    }
    return neighbors;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < u->rows; ++i) { //iterate through rows
        for (uint32_t j = 0; j < u->cols; ++j) { //iterate through cols of each row
            if (uv_get_cell(u, i, j)) { //if living print an o
                fprintf(outfile, "o");
            } else { //if dead pring a .
                fprintf(outfile, ".");
            }
            //if on the last column print another line
            if (j == u->cols - 1) {
                fprintf(outfile, "\n");
            }
        }
    }
}
