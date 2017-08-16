#include "grid.h"

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
