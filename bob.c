#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "validate.h"


/**
 * Play through a game until the game is over then print the winner
 */
int play_game(Game* game) {
    
    print_grid(game->grid);

    while (!game->isOver(game)) {
        game->move(game);
        // Stop when the game is forcefully ended.
        if (game->over) {
            return 6;
        }
        print_grid(game->grid);
    }
    
    // Determine the winner of the game and print the result
    int winner = game->winner(game);
    
    if (winner != 0) {
        printf("Player %c wins\n", winner == 1 ? 'O' : 'X');
    }

    return 0;
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
    Game* game;

    int player1 = argv[1][0] == 'a' ? 0 : 1;
    int player2 = argv[2][0] == 'a' ? 0 : 1;

    if (argc == 4) {
        // Loads the grid from a file.
        FILE* file = fopen(argv[3], "r");
        grid = load_grid(file, dimensions[0], dimensions[1]);
        rewind(file);
        game = load_game(file, grid, player1, player2);
    } else if(argc == 5) {
        // Creates a game and grid based on parameters provided.
        grid = create_grid(dimensions[0], dimensions[1]);
        game = create_game(grid, player1, player2);
    }

    if (play_game(game) == 6) {
        fprintf(stderr, "EOF from user\n");
        return 6;
    }

    return 0;
}

