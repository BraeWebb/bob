#include <stdio.h>

#include "grid.h"
#include "validate.h"

int main(int argc, char** argv){

    int error_code = validate(argc, argv);
    if(error_code != 0){
        return error_code;
    }

    printf("%c\n", argv[1][0]);
    printf("%c\n", argv[2][0]);

    Grid* grid = createGrid(10, 7);
    grid->set(grid, 0, 0, PLAYER1);
    grid->set(grid, 5, 5, PLAYER1);
    grid->set(grid, 7, 3, PLAYER2);
    
    printf("%d\n", grid->get(grid, 7, 3));
    printf("%d\n", grid->get(grid, 7, 4));

    printGrid(grid);

    printf("\n%s\n", grid->serialize(grid));

    return 0;
}
