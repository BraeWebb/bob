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

int isInGrid(Grid* grid, int x, int y){
    if(x >= 0 && y >= 0){
        if(x < grid->rows && y < grid->columns){
            return 1;
        }
    }
    return 0;
}

int** getNeighbours(Grid* grid, int x, int y){
    
    // there can be a maximum of 6 neighbours
    int** neighbours = malloc(sizeof(int*) * 6);

    int index = 0;
    
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(abs(i - j) <= 1 && !(i == 0 && j == 0)){
                
                int neighbourX = x+i;
                int neighbourY = y+j;
            
                if(isInGrid(grid, neighbourX, neighbourY)){
                    int* position = malloc(sizeof(int) * 2);
                    
                    position[0] = neighbourX;
                    position[1] = neighbourY;
                    
                    neighbours[index] = position;
                } else {
                    neighbours[index] = NULL;
                }

                index++;
            }
        }
    }

    return neighbours;
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
    grid->findNeighbours = getNeighbours;    

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
