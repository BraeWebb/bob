#include <stdlib.h>
#include <stdio.h>

#include "grid.h"

char convertCharacter(Marker marker){
    char markers[3] = {'.', 'O', 'X'};
    
    return markers[marker];
}

void setGridValue(Grid* grid, int x, int y, Marker value){
    grid->values[x][y] = value;
}

Marker getGridValue(Grid* grid, int x, int y){
    return grid->values[x][y];
}

char* serializeGrid(Grid* grid){
    char* string = malloc(sizeof(Marker) * grid->rows * (grid->columns + 1));
    
    for(int x = 0; x < grid->rows; x++){
        for(int y = 0; y < grid->columns; y++){
            string[grid->columns * x + y] = convertCharacter(grid->get(grid, x, y));
            //printf("%d: %c\n", grid->rows * x + y, convertCharacter(grid->get(grid, x, y)));
        }
        string[grid->columns * x + grid->rows] = '\n';
    }
    
    return string;
}

Grid* createGrid(int rows, int columns){
    Marker** values = malloc(sizeof(Marker *) * rows);
    
    for(int i = 0; i < rows; i++){
        values[i] = malloc(sizeof(Marker) * columns);
    }
    
    Grid* grid = malloc(sizeof(Grid));
    
    grid->rows = rows;
    grid->columns = columns;
    grid->values = values;

    grid->set = setGridValue;
    grid->get = getGridValue;
    grid->serialize = serializeGrid;
    
    return grid;
}

void printGrid(Grid* grid){
    for(int x = 0; x < grid->rows; x++){
        for(int space = 1; space < grid->rows - x; space++){
            printf(" ");
        }
        for(int y = 0; y < grid->columns; y++){
            printf("%c ", convertCharacter(grid->values[x][y]));
        }
        printf("\n");
    }
}
