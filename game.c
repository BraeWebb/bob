#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "game.h"

int* prompt(Game* game) {

    int* move = malloc(sizeof(int) * 2);

    while (1) {
        printf("Player %c] ", game->turn ? 'X' : 'O');
        
        int fail = 0;
        int next = 0;
        int position = 0;
        int spaces = 0;
        int saving = 0;
        int savePosition = 0;
        int saveFileLength = 70;
        char* saveFile = malloc(sizeof(char) * saveFileLength);
        
        move[0] = 0;
        move[1] = 0;
    
        while (1) {
            next = fgetc(stdin);
            if (next == '\n') {
                break;
            }
            if (next == 's') {
                if(position == 0) {
                    saving = 1;
                    continue;
                }
            }
            if (next == ' ') {
                spaces++;
                if(spaces > 1) {
                    fail = 1;
                    break;
                }
                continue;
            }
            if (saving) {
                if(savePosition > saveFileLength) {
                    saveFileLength = saveFileLength + 70;
                    saveFile = realloc(saveFile, saveFileLength);
                }
                saveFile[savePosition++] = next;
            } else {
                if(isdigit(next)) {
                    move[spaces] = move[spaces] * 10 + (next - '0');
                    if (move[0] >= game->grid->rows
                            || move[1] >= game->grid->columns) {
                        fail = 1;
                        break;
                    }
                } else {
                    fail = 1;
                    break;
                }
            }
            position++;
        }

        printf("%s\n", saveFile);

        fflush(stdin);

        if (fail) {
            continue;
        }

        return move;
    }
}


/**
 * Uses the algorithm described below to determine the position of an
 * automatic players next move based on the state of the game.
 *
 * Returns an array of length two containing the x and y positions respectively
 *
 * Algorithm:
 *      let n be number of moves of current automatic player
 *      let t be an intermediate value
 *      let height be the amount of rows in the grid
 *      let width be the amount of columns in the grid
 *      let m be the greatest value of height and width
 *      then
 *          if player is player1
 *              t = (n * 9 % 1000037) + 17
 *          otherwise
 *              t = (n * 7 % 1000213) + 81
 *
 *          x = (t / m) mod height
 *          y = t mod width
 *      if the position is taken
 *          increment number of moves and retry
 */
int* automatic_move(Game* game) {
    int gridHeight = game->grid->rows;
    int gridWidth = game->grid->columns;

    int maxDimension = gridHeight > gridWidth ? gridHeight : gridWidth;

    int moves, intermediate, row, column;

    while (1) {
        
        moves = game->moves[game->turn];

        if (game->turn == 0) {
            intermediate = ((moves * 9 % 1000037) + 17);
        } else {
            intermediate = ((moves * 7 % 1000213) + 81);
        }
    
        row = (intermediate / maxDimension) % gridHeight;
        column = intermediate % gridWidth;

        if (game->grid->get(game->grid, row, column) != 0) {
            game->moves[game->turn]++;
        } else {
            break;
        }
    }

    int* result = malloc(sizeof(int) * 2);
    result[0] = row;
    result[1] = column;

    return result;
}

/**
 * Determines the winner, if any, of the game based on the games state.
 *
 * Returns 
 *      0 iff no winner.
 *      1 iff player 1 wins.
 *      2 iff player2 wins.
 * 
 */
int get_winner(Game* game) {
    // The maximum possible amount of positions in a trail through the grid.
    int maxSize = game->grid->rows * game->grid->columns;
    
    // Determine if player 1 is a winner.
    // Looks from the first position in each row and checks if a path
    // can be made to a position in the last column.
    for (int x = 0; x < game->grid->rows; x++) {
        // Only check player 1 tiles.
        if (game->grid->get(game->grid, x, 0) == 1) {
            // Find all tiles connected to the origin.
            int** matches = game->grid->search(game->grid, x, 0);
            
            // Return 1 if any of the connected tiles are in the last column.
            for (int i = 0; i < maxSize; i++) {
                if (matches[i] != NULL) {
                    if (matches[i][1] == game->grid->columns - 1) {
                        return 1;
                    }
                }
            }
        }
    }

    // Determine if player 2 is a winner.
    // Looks from the first position in each column and checks if a path can be
    // made to a position in the last row.
    for (int y = 0; y < game->grid->columns; y++) {
        // Only check player 2 tiles.
        if (game->grid->get(game->grid, 0, y) == 2) {
            // Find all tiles connected to the origin.
            int** matches = game->grid->search(game->grid, 0, y);
            
            // Return 1 if any of the connected tiles are in the last row.
            for (int i = 0; i < maxSize; i++) {
                if (matches[i] != NULL) {
                    if (matches[i][0] == game->grid->rows - 1) {
                        return 2;
                    }
                }
            }
        }
    }

    return 0;
}

/**
 * Checks to see if the game is over by checking if there is a winner or
 * if the grid is full.
 *
 * Returns 1 iff the game is over, otherwise 0.
 *
 */
int is_game_over(Game* game) {
    // Determine if the game has a winner
    if (get_winner(game) != 0) {
        return 1;
    }

    // Determine if the grid is full
    for (int x = 0; x < game->grid->rows; x++) {
        for (int y = 0; y < game->grid->columns; y++) {
            if (game->grid->get(game->grid, x, y) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * Makes the next move based on the current state of the game.
 *
 * Either prompts user for input if the next turn is a manual player or
 * generates the next move of the automatic player.
 * Then takes those positions and makes that move.
 *
 */
void take_turn(Game* game) {
    int* move = malloc(sizeof(int) * 2);

    if (game->modes[game->turn] == 0) {
        // If it's an automatic players turn then generate the move.
        move = automatic_move(game);
        char player = game->turn ? 'X' : 'O';
        printf("Player %c => %d %d\n", player, move[0], move[1]);
    } else {
        // If it's a manual players turn then prompt for their move.
        move = prompt(game);
    }
    // Update the game based on the selected move.
    game->grid->set(game->grid, move[0], move[1], (game->turn + 1));
    game->moves[game->turn]++;
    game->turn = game->turn ? 0 : 1;
}


/**
 * Creates a new Game struct and sets the values of it's properties
 *
 * Returns a pointer to the new Game struct.
 *
 */
Game* create_game(Grid* grid, int player1Mode, int player2Mode) {
    Game* game = malloc(sizeof(Game));
    
    // Allocates space for the player modes and the amount of moves made.
    int* modes = malloc(sizeof(int) * 2);
    int* moves = calloc(2, sizeof(int));

    modes[0] = player1Mode;
    modes[1] = player2Mode;
    
    // Initializes struct properties
    game->turn = 0;
    game->modes = modes;
    game->moves = moves;
    game->grid = grid;

    // Assigns local functions to struct properties for a 'class' experience.
    game->move = take_turn;
    game->isOver = is_game_over;
    game->winner = get_winner;
    
    return game;
}
