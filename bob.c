#include <stdio.h>
#include <string.h>

#include "grid.h"

#define USAGE "Usage: bob p1type p2type [height width | filename]\n"
#define INVALID_TYPE "Invalid type\n"

int main(int argc, char** argv){
    
    if(argc < 3){
        printf(USAGE);
        return 1;
    }
    
    if(strlen(argv[1]) != 1 || strlen(argv[2]) != 1){
        printf(INVALID_TYPE);
        return 2;
    }

    if((argv[1][0] != 'a' && argv[1][0] != 'm')
        || (argv[2][0] != 'a' && argv[2][0] != 'm')){
        printf(INVALID_TYPE);
        return 2;
    }

    printf("%c\n", argv[1][0]);
    printf("%c\n", argv[2][0]);

    Grid* grid = createGrid(10, 7);
    grid->set(grid, 0, 0, p1);
    grid->set(grid, 5, 5, p1);
    grid->set(grid, 7, 3, p2);
    
    printf("%d\n", grid->get(grid, 7, 3));
    printf("%d\n", grid->get(grid, 7, 4));

    printGrid(grid);

    return 0;
}
