#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "validate.h"

#define USAGE "Usage: bob p1type p2type [height width | filename]\n"
#define INVALID_TYPE "Invalid type\n"
#define GRID_DIMENSIONS "Sensible board dimensions please!\n"
#define FILE_OPEN "Could not start reading from savefile\n"
#define FILE_READ "Incorrect file contents\n"

int validate(int argc, char** argv){
    
    if(argc < 3 || argc > 5){
        printf(USAGE);
        return 1;
    }
    
    if(strlen(argv[1]) != 1 || strlen(argv[2]) != 1){
        printf(INVALID_TYPE);
        return 2;
    }

    if((argv[1][0] != 'a' && argv[1][0] != 'm')
        || (argv[2][0] != 'a' && argv[2][0] != 'm')){
        printf(INVALID_TYPE);
        return 2;
    }

    if(argc == 5){
        errno = 0;
        char* p;
        
        strtol(argv[3], &p, 10);
        
        if(errno != 0 || *p != '\0'){
            printf(GRID_DIMENSIONS);
            return 3;
        }
        
        strtol(argv[4], &p, 10);
        
        if(errno != 0 || *p != '\0'){
            printf(GRID_DIMENSIONS);
            return 3;
        }
    }

    if(argc == 4){
        FILE* file = fopen(argv[3], "r");
        if (file == NULL){
            printf(FILE_OPEN);
            return 4;
        }
        
        int next = 0;
        int readingGrid = 0;
        int columns = 0;
        int rows = 0;
        int currentColumn = 0;
        int gameDataCount = 0;
        int gameCommaCount = 0;
        char lastGameChar = ',';
        int character = 0;
        
        while(1){
            character++;
            next = fgetc(file);
            if (next == '\n'){
                if(!readingGrid){
                    readingGrid = 1;
                } else {
                    if (currentColumn != columns){
                        printf(FILE_READ);
                        return 5;
                    }
                    currentColumn = 0;
                    rows++;
                }
                continue;
            } 
            if (next == EOF){
                break;
            }
            if(!readingGrid){
                if(next == ','){
                    if(lastGameChar == ','){
                        printf(FILE_READ);
                        return 5;
                    }
                    gameCommaCount++;
                } else {
                    if(!isdigit(next)){
                        printf(FILE_READ);
                        return 5;
                    }
                    gameDataCount++;
                }
                lastGameChar = (char) next;
            } else {
                if (next == '.' || next == 'O' || next == 'X'){
                    if (rows == 0){
                        columns++;
                    }
                    currentColumn++;
                } else {
                    printf(FILE_READ);
                    return 5;
                }
            }
        }
        
        if (!readingGrid || columns == 0 || rows == 0){
            printf(FILE_READ);
            return 5;
        }
        if (gameDataCount < 5 || gameCommaCount != 4){
            printf(FILE_READ);
            return 5;
        }
    }

    return 0;
}
