#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "validate.h"

int main(int argc, char** argv){

    int error_code = validate(argc, argv);
    if(error_code != 0){
        return error_code;
    }

    Grid* grid;

    if(argc == 5){
        char* p;
        long rows = strtol(argv[3], &p, 10);
        long columns = strtol(argv[4], &p, 10);
        grid = createGrid(rows, columns);
    } else {
        grid = createGrid(10, 7);
    }

    Game* game = createGame(grid, 0, 0);

    printGrid(grid);
    
    while (1) {
        game->move(game);
        printGrid(grid);
        if(game->isOver(game)){
            break;
        }
    }

    int** searched = grid->search(grid, 0, 0);
    for(int i = 0; i < (grid->rows * grid->columns); i++){
        if(searched[i] != NULL){
            printf("-> [%d, %d] ", searched[i][0], searched[i][1]);
        }
    }
    printf("\n");

    return 0;
}
