#include <stdio.h>

#include "grid.h"

int main(int argc, char** argv){
    
    Grid* grid = createGrid(10, 7);
    grid->values[0][0] = p1;
    grid->values[5][5] = p1;
    grid->values[7][3] = p2;
    printGrid(grid);

    return 0;
}
