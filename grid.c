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

int positionInArray(int* position, int** array, int size){
    for(int i = 0; i < size; i++){
        if(array[i] != NULL){
            if(array[i][0] == position[0] && array[i][1] == position[1]){
                return 1;
            }
        }   
    }
    return 0;
}

int** searchGrid(Grid* grid, int startX, int startY){
    // Theoretically it shouldn't need to search more than half the grid...
    // but just to be safe
    int maxSize = grid->rows * grid->columns;
    int** nodes = malloc(sizeof(int*) * maxSize);
    int** visited = malloc(sizeof(int*) * maxSize);
    
    for(int i = 0; i < maxSize; i++){
        nodes[i] = NULL;
        visited[i] = NULL;
    }

    int type = grid->get(grid, startX, startY);
    
    nodes[0] = malloc(sizeof(int) * 2);
    nodes[0][0] = startX;
    nodes[0][1] = startY;
    
    int nodeCount = 1;
    int visitedCount = 0;

    while(nodeCount > 0){
        int* node = nodes[--nodeCount];
        
        int x = node[0];
        int y = node[1];

        if(grid->get(grid, x, y) == type){
            if(!positionInArray(node, visited, maxSize)){
                
                int** neighbours = grid->findNeighbours(grid, x, y);
                
                for(int i = 0; i < 6; i++){
                    if(neighbours[i] != NULL){
                        nodes[nodeCount++] = neighbours[i];
                    }
                }
                
                visited[visitedCount++] = node;
            }
        }
    }

    return visited;
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


Grid* createGridMethods(Grid* grid){
    grid->set = setGridValue;
    grid->get = getGridValue;
    grid->serialize = serializeGrid;
    grid->findNeighbours = getNeighbours;
    grid->search = searchGrid;

    return grid;
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

    grid = createGridMethods(grid);

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

Grid* loadGrid(FILE* file, int rows, int columns){

    Grid* grid = createGrid(rows, columns);

    Marker** values = grid->values;
    
    int started = 0;
    int x = 0;
    int y = 0;
    int next = 0;
    Marker marker;

    while (1) {
        next = fgetc(file);
        if (next == EOF){
            break;
        }
        if (next == '\n'){
            if (!started){
                started = 1;
            } else {
                x++;
                y = 0;
            }
            continue;
        }
        if (!started){
            continue;
        }
        if (next == 'O'){
            marker = PLAYER1;
        } else if (next == 'X'){
            marker = PLAYER2;
        } else if (next == '.'){
            marker = EMPTY;
        }
        values[x][y] = marker;
        y++;
    }
    
    return grid;
}
