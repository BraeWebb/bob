#include <stdio.h>

#include "grid.h"

int main(int argc, char** argv){
    
    Grid* grid = createGrid(10, 7);
    grid->set(grid, 0, 0, p1);
    grid->set(grid, 5, 5, p1);
    grid->set(grid, 7, 3, p2);
    
    printf("%d\n", grid->get(grid, 7, 3));
    printf("%d\n", grid->get(grid, 7, 4));

    printGrid(grid);

    return 0;
}
