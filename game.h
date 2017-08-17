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

    void (*move)(struct Game*);
    int (*isOver)(struct Game*);
    int (*winner)(struct Game*);
} Game;

Game* create_game(Grid* grid, int player1Mode, int player2Mode);
