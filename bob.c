#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "validate.h"

int main(int argc, char** argv){

    int* dimensions = malloc(sizeof(int) * 2);

    int error_code = validate(argc, argv, dimensions);
    if(error_code != 0){
        return error_code;
    }

    Grid* grid;

    if(argc == 4){
        FILE* file = fopen(argv[3], "r");
        grid = loadGrid(file, dimensions[0], dimensions[1]);
    }else if(argc == 5){
        grid = createGrid(dimensions[0], dimensions[1]);
    } else {
        grid = createGrid(10, 7);
    }
    
    Game* game = createGame(grid, argv[1][0] == 'a' ? 0 : 1, 
                            argv[2][0] == 'a' ? 0 : 1);

    printGrid(grid);
    
    while (1) {
        game->move(game);
        printGrid(grid);
        if(game->isOver(game)){
            break;
        }
    }
    int winner = game->winner(game);
    if(winner != 0){
        printf("Player %c wins\n", winner == 1 ? 'O' : 'X');
    } else {
        printf("Failure!\n");
    }
    return 0;
}
