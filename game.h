#include "grid.h"

/**
 * A Game structure which keeps track of the current game state.
 *
 * Includes the move, is_over and winner methods.
 *
 */
typedef struct game {
    int turn;
    int* modes;
    int* moves;
    Grid* grid;

    void (*move)(struct game*);
    int (*is_over)(struct game*);
    int (*winner)(struct game*);
} Game;

Game* create_game(Grid* grid, int player1Mode, int player2Mode);
