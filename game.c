#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "game.h"

int* prompt(Game* game) {

    int* move = malloc(sizeof(int) * 2);

    while (1) {
        printf("Player %c] ", game->turn ? 'X' : 'O');
        
        int spaces = 0;
        int setX = 0;
        int setY = 0;
        int badInput = 0;
        int maxLineLength = 70;
        char* saveFile = malloc(sizeof(char) * maxLineLength);
        
        move[0] = 0;
        move[1] = 0;
    
        char* output = malloc(sizeof(char) * maxLineLength);

        char* result = fgets(output, maxLineLength, stdin);
        
        if (result == NULL) {
            game->over = 1;
            return NULL;
        }

        if (output[0] == 's') {
            // Chop off the leading s and the trailing new line.
            strncpy(saveFile, output + 1, strlen(output) - 2);
            
            game->save(game, saveFile);
 
            continue;
        } else {
            for (int i = 0; i < maxLineLength; i++) {
                if (output[i] == '\n') {
                    break;
                }
                if (output[i] == '\0') {
                    printf("\n");
                    break;
                }
                if (output[i] == ' ') {
                    spaces++;
                    if (spaces > 1) {
                        break;
                    }
                } else if (isdigit(output[i])) {
                    if (spaces == 0) {
                        setX = 1;
                    } else if (spaces == 1) {
                        setY = 1;
                    }
                    move[spaces] = (move[spaces] * 10) + (output[i] - '0');
                } else {
                    badInput = 1;
                    break;
                }
            }
        }

        if (badInput || spaces > 1 || !setX || !setY) {
            continue;
        }

        if (move[0] >= game->grid->rows || move[1] >= game->grid->rows) {
            continue;
        }

        if (game->grid->get(game->grid, move[0], move[1]) != 0) {
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
    // Ensure that the game has not been forcefully ended.
    if (game->over) {
        return 1;
    }

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
        game->moves[game->turn]++;
    } else {
        // If it's a manual players turn then prompt for their move.
        move = prompt(game);
        if (move == NULL) {
            return;
        }
    }
    // Update the game based on the selected move.
    game->grid->set(game->grid, move[0], move[1], (game->turn + 1));
    game->turn = !game->turn;
}

/**
 * Saves the current state of a game and a grid into a specified file.
 */
void save_game(Game* game, char* filename) {
    // NOTE: I've had to open the file here because style.sh rejects
    // having a file pointer in header files
    FILE* file = fopen(filename, "w");
    fprintf(file, "%d,%d,%d,%d,%d\n", game->turn, game->grid->rows, 
            game->grid->columns, game->moves[0], game->moves[1]);
    fclose(file);
    game->grid->save(game->grid, filename);
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
    game->over = 0;

    // Assigns local functions to struct properties for a 'class' experience.
    game->move = take_turn;
    game->isOver = is_game_over;
    game->winner = get_winner;
    game->save = save_game;
    
    return game;
}


/**
 * Creates a new game and loads it with information from a file.
 *
 * Returns NULL if an error occurs loading the game, otherwise it returns
 * the newly created Game.
 */
Game* load_game(FILE* file, Grid* grid, int player1Mode, int player2Mode) {
    Game* game = create_game(grid, player1Mode, player2Mode);

    int next = 0;
    int value = 0;
    int* values = calloc(5, sizeof(int));

    while (1) {
        next = fgetc(file);
        
        if (next == '\n' || next == EOF) {
            break;
        }
        
        if (next == ',') {
            value++;
            continue;
        }

        values[value] = (values[value] * 10) + (next - '0');
    }

    if (values[1] != grid->rows || values[2] != grid->columns) {
        return NULL;
    }

    game->turn = values[0];
    game->moves[0] = values[3];
    game->moves[1] = values[4];

    return game;
}
