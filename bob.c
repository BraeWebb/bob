#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "validate.h"

int main(int argc, char** argv) {

    int* dimensions = malloc(sizeof(int) * 2);

    int errorCode = validate(argc, argv, dimensions);
    if (errorCode != 0) {
        return errorCode;
    }

    Grid* grid;

    if (argc == 4) {
        FILE* file = fopen(argv[3], "r");
        grid = load_grid(file, dimensions[0], dimensions[1]);
    } else if(argc == 5) {
        grid = create_grid(dimensions[0], dimensions[1]);
    } else {
        grid = create_grid(10, 7);
    }
    
    Game* game = create_game(grid, argv[1][0] == 'a' ? 0 : 1, 
                                argv[2][0] == 'a' ? 0 : 1);

    print_grid(grid);
    
    while (1) {
        game->move(game);
        print_grid(grid);
        if (game->is_over(game)) {
            break;
        }
    }
    int winner = game->winner(game);
    if (winner != 0) {
        printf("Player %c wins\n", winner == 1 ? 'O' : 'X');
    } else {
        printf("Failure!\n");
    }
    return 0;
}
