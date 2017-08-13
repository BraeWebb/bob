#include <stdlib.h>
#include <stdio.h>

#include "game.h"

int* prompt(int turn) {
    printf("Player %c] ", turn ? 'O' : 'X');
    printf("\n");
    int* move = malloc(sizeof(int) * 2);

    move[0] = 0;
    move[1] = 0;

    return move;
}

int* automaticMove(Game* game){
    int gridHeight = game->grid->rows;
    int gridWidth = game->grid->columns;

    int m = gridHeight > gridWidth ? gridHeight : gridWidth;

    int n, t, row, column;    

    while (1) {
        
        n = game->moves[game->turn];

        if (game->turn == 0){
            t = ((n * 9 % 1000037) + 17);
        } else {
            t = ((n * 7 % 1000213) + 81);
        }
    
        row = (t/m) % gridHeight;
        column = t % gridWidth;

        if (game->grid->get(game->grid, row, column) != 0){
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

int isGameOver(Game* game){
    for (int x = 0; x < game->grid->rows; x++){
        for (int y = 0; y < game->grid->columns; y++){
            if (game->grid->get(game->grid, x, y) == 0){
                return 0;
            }
        }
    }
    return 1;
}

void takeTurn(Game* game){
    int* move = malloc(sizeof(int) * 2);

    if (game->modes[game->turn] == 0){
        move = automaticMove(game);
        printf("Player %c => %d %d\n", game->turn ? 'O' : 'X', move[0], move[1]);
    } else {
        move = prompt(game->turn);
    }
    game->grid->set(game->grid, move[0], move[1], (game->turn + 1));
    game->moves[game->turn]++;
    game->turn = game->turn ? 0 : 1;
}

Game* createGame(Grid* grid, int player1Mode, int player2Mode){
    Game* game = malloc(sizeof(Game));
    
    int* modes = malloc(sizeof(int) * 2);
    int* moves = calloc(2, sizeof(int));

    modes[0] = player1Mode;
    modes[1] = player2Mode;
    
    game->turn = 0;
    game->modes = modes;
    game->moves = moves;
    game->grid = grid;

    game->move = takeTurn;
    game->isOver = isGameOver;
    
    return game;
}
