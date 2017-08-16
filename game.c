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
            if (next == '\n'){
                break;
            }
            if (next == 's'){
                if(position == 0){
                    saving = 1;
                    continue;
                }
            }
            if (next == ' '){
                spaces++;
                if(spaces > 1){
                    fail = 1;
                    break;
                }
                continue;
            }
            if (saving){
                if(savePosition > saveFileLength){
                    saveFileLength = saveFileLength + 70;
                    saveFile = realloc(saveFile, saveFileLength);
                }
                saveFile[savePosition++] = next;
            } else {
                if(isdigit(next)){
                    move[spaces] = move[spaces] * 10 + (next - '0');
                    if (move[0] >= game->grid->rows
                        || move[1] >= game->grid->columns){
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

        if(fail){
            continue;
        }

        return move;
    }
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

int getWinner(Game* game){
    int maxSize = game->grid->rows * game->grid->columns;
    
    for(int x = 0; x < game->grid->rows; x++){
        if(game->grid->get(game->grid, x, 0) == 1){
            int** matches = game->grid->search(game->grid, x, 0);
            
            for(int i = 0; i < maxSize; i++){
                if(matches[i] != NULL){
                    if(matches[i][1] == game->grid->columns - 1){
                        return 1;
                    }
                }
            }
        }
    }

    for(int y = 0; y < game->grid->columns; y++){
        if(game->grid->get(game->grid, 0, y) == 2){
            int** matches = game->grid->search(game->grid, 0, y);

            for(int i = 0; i < maxSize; i++){
                if(matches[i] != NULL){
                    if(matches[i][0] == game->grid->rows - 1){
                        return 2;
                    }
                }
            }
        }
    }

    return 0;
}

int isGameOver(Game* game){
    if(getWinner(game) != 0){
        return 1;
    }

    // Determine if the grid is full
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
        printf("Player %c => %d %d\n", game->turn ? 'X' : 'O', move[0], move[1]);
    } else {
        move = prompt(game);
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
    game->winner = getWinner;
    
    return game;
}
