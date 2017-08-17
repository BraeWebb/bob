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

int error(int errorCode) {
    switch (errorCode) {
        case 1:
            fprintf(stderr, USAGE);
            return 1;
        case 2:
            fprintf(stderr, INVALID_TYPE);
            return 2;
        case 3:
            fprintf(stderr, GRID_DIMENSIONS);
            return 3;
        case 4:
            fprintf(stderr, FILE_OPEN);
            return 4;
        case 5:
            fprintf(stderr, FILE_READ);
            return 5;
        default:
            return errorCode;
    } 
}

int validate(int argc, char** argv, int* dimensions) {
    
    if (argc < 4 || argc > 5) {
        return error(1);
    }
    
    if (strlen(argv[1]) != 1 || strlen(argv[2]) != 1) {
        return error(2);
    }

    if ((argv[1][0] != 'a' && argv[1][0] != 'm')
        || (argv[2][0] != 'a' && argv[2][0] != 'm')) {
        return error(2);
    }

    if (argc == 5) {
        errno = 0;
        char* p;
        
        dimensions[0] = strtol(argv[3], &p, 10);
        
        if (errno != 0 || *p != '\0') {
            return error(3);
        }
        
        dimensions[1] = strtol(argv[4], &p, 10);
        
        if (errno != 0 || *p != '\0') {
            return error(3);
        }

        if (dimensions[0] <= 0 || dimensions[0] > 1000 
            || dimensions[1] <= 0 || dimensions[1] > 1000) {
            return error(3);
        }
    }

    if (argc == 4) {
        FILE* file = fopen(argv[3], "r");
        if (file == NULL) {
            return error(4);
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
        
        while(1) {
            character++;
            next = fgetc(file);
            if (next == '\n') {
                if (!readingGrid) {
                    readingGrid = 1;
                } else {
                    if (currentColumn != columns) {
                        return error(5);
                    }
                    currentColumn = 0;
                    rows++;
                }
                continue;
            } 
            if (next == EOF) {
                break;
            }
            if (!readingGrid) {
                if (next == ',') {
                    if (lastGameChar == ',') {
                        return error(5);
                    }
                    gameCommaCount++;
                } else {
                    if (!isdigit(next)) {
                        return error(5);
                    }
                    gameDataCount++;
                }
                lastGameChar = (char) next;
            } else {
                if (next == '.' || next == 'O' || next == 'X') {
                    if (rows == 0) {
                        columns++;
                    }
                    currentColumn++;
                } else {
                    return error(5);
                }
            }
        }
        
        if (!readingGrid || columns == 0 || rows == 0) {
            return error(5);
        }
        if (gameDataCount < 5 || gameCommaCount != 4) {
            return error(5);
        }
        dimensions[0] = rows;
        dimensions[1] = columns;
    }

    return 0;
}
