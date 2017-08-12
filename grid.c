#include <stdlib.h>
#include <stdio.h>

#include "grid.h"

char convertCharacter(int character){
    switch (character) {
        case 0: 
            return '.';
        case 1:
            return 'O';
        case 2:
            return 'X';
        default:
            return '?';
    }
}

void setGridValue(Grid* grid, int x, int y, int value){
    grid->values[x][y] = value;
}

int getGridValue(Grid* grid, int x, int y){
    return grid->values[x][y];
}

Grid* createGrid(int rows, int columns){
    int** values = malloc(sizeof(int *) * rows);
    
    for(int i = 0; i < rows; i++){
        values[i] = malloc(sizeof(int) * columns);
    }
    
    Grid* grid = malloc(sizeof(Grid));
    
    grid->rows = rows;
    grid->columns = columns;
    grid->values = values;

    grid->set = setGridValue;
    grid->get = getGridValue;
    
    return grid;
}

void printGrid(Grid* grid){
    for(int x = 0; x < grid->rows; x++){
        for(int space = 0; space < grid->rows - x; space++){
            printf(" ");
        }
        for(int y = 0; y < grid->columns; y++){
            printf(" %c ", convertCharacter(grid->values[x][y]));
        }
        printf("\n");
    }
}
