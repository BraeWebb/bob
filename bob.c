#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "validate.h"


/**
 * Play through a game until the game is over then print the winner
 */
void play_game(Game* game) {
    
    print_grid(game->grid);

    while (!game->is_over(game)) {
        game->move(game);
        print_grid(game->grid);

    }
    
    // Determine the winner of the game and print the result
    int winner = game->winner(game);
    
    if (winner != 0) {
        printf("Player %c wins\n", winner == 1 ? 'O' : 'X');
    } else {
        printf("Nobody won\n");
    }
}

/**
 * Handles the main running of the bob program.
 *
 * Takes the amount of arguments passed to the program and a list of the
 * actual arguments passed.
 *
 */
int main(int argc, char** argv) {
    
    // Stores the dimensions calculated from the validate function.
    int* dimensions = malloc(sizeof(int) * 2);

    // Calls the validate function which checks that all the arguments given
    // are correct and sets the board dimensions.
    int errorCode = validate(argc, argv, dimensions);

    if (errorCode != 0) {
        return errorCode;
    }

    Grid* grid;

    if (argc == 4) {
        // Loads the grid from a file.
        FILE* file = fopen(argv[3], "r");
        grid = load_grid(file, dimensions[0], dimensions[1]);
    } else if(argc == 5) {
        // Creates a grid based on provided dimensions.
        grid = create_grid(dimensions[0], dimensions[1]);
    } else {
        // TODO: Check if this is allowed behaviour
        grid = create_grid(10, 7);
    }
    
    // Creates a game based off the grid and the types of players requested.
    Game* game = create_game(grid, argv[1][0] == 'a' ? 0 : 1, 
                                argv[2][0] == 'a' ? 0 : 1);

    play_game(game);

    return 0;
}

