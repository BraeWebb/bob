#include "grid.h"

typedef struct game {
    int turn;
    int* modes;
    int* moves;
    Grid* grid;

    void (*move)(struct game*);
    int (*isOver)(struct game*);
} Game;

Game* createGame(Grid* grid, int player1Mode, int player2Mode);
