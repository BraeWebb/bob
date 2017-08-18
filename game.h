#include "grid.h"

/**
 * A Game structure which keeps track of the current game state.
 *
 * Includes the move, is_over and winner methods.
 *
 */
typedef struct Game {
    int turn;
    int* modes;
    int* moves;
    Grid* grid;
    int over;

    void (*move)(struct Game*);
    int (*isOver)(struct Game*);
    int (*winner)(struct Game*);
    // For some reason the style.sh rejects a file pointer...
    void (*save)(struct Game*, char* filename);
} Game;

Game* create_game(Grid* grid, int player1Mode, int player2Mode);

Game* load_game(FILE* file, Grid* grid, int player1Mode, int player2Mode);
