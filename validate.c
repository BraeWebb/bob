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

/**
 * Takes an error code and prints the appropriate error message to stderr.
 *
 * Returns the original error code.
 */
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

/**
 * Reads a game save file to determine if the file is formatted correctly.
 *
 * Returns NULL if the file is formatted incorrectly, otherwise returns the
 * dimensions of the grid in the file.
 */
int* validate_file(FILE* file){
    
    int next = 0;
    int* dimensions = malloc(sizeof(int) * 2);
    int readingGrid = 0;
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
                if (currentColumn != dimensions[1]) {
                    return NULL;
                }
                currentColumn = 0;
                dimensions[0]++;
            }
            continue;
        } 
        if (next == EOF) {
            break;
        }
        if (!readingGrid) {
            if (next == ',') {
                if (lastGameChar == ',') {
                    return NULL;
                }
                gameCommaCount++;
            } else {
                if (!isdigit(next)) {
                    return NULL;
                }
                gameDataCount++;
            }
            lastGameChar = (char) next;
        } else {
            if (next == '.' || next == 'O' || next == 'X') {
                if (dimensions[0] == 0) {
                    dimensions[1]++;
                }
                currentColumn++;
            } else {
                return NULL;
            }
        }
    }
    
    if (!readingGrid || dimensions[0] == 0 || dimensions[1] == 0) {
        return NULL;
    }
    if (gameDataCount < 5 || gameCommaCount != 4) {
        return NULL;
    }
    
    return dimensions;
}

/**
 * Determines if two arguments are valid grid dimensions.
 *
 * Returns NULL iff the arguments are invalid, else returns the dimensions.
 */
int* validate_dimensions(char* arg1, char* arg2) {
    errno = 0;
    char* p;
    
    // Reserve space for the dimensions.
    int* dimensions = malloc(sizeof(int) * 2);

    // Attempts to convert the first argument.
    dimensions[0] = strtol(arg1, &p, 10);

    if (errno != 0 || *p != '\0') {
        return NULL;
    }
    
    // Attempts to convert the second argument.
    dimensions[1] = strtol(arg2, &p, 10);

    if (errno != 0 || *p != '\0') {
        return NULL;
    }

    // Ensure that both coordinates are reasonable.
    if (dimensions[0] <= 0 || dimensions[0] > 1000) {
        return NULL;
    }

    if (dimensions[1] <= 0 || dimensions[1] > 1000) {
        return NULL;
    }
    
    return dimensions;
}


/**
 * Checks program arguments to insure that valid input is given.
 *
 * Returns 0 iff there is no error, otherwise returns the error code.
 *
 * Updates the given dimensions pointer to contain the dimensions of the grid.
 */
int validate(int argc, char** argv, int* dimensions) {
   
    // Ensure the amount of arguments is correct. 
    if (argc < 4 || argc > 5) {
        return error(1);
    }
    
    // Ensure that the first two arguments only have one character.
    if (strlen(argv[1]) != 1 || strlen(argv[2]) != 1) {
        return error(2);
    }

    // Ensure the first two arguments are either 'a' or 'm'.
    if (argv[1][0] != 'a' && argv[1][0] != 'm') {
        return error(2);
    }
    
    if (argv[2][0] != 'a' && argv[2][0] != 'm') {
        return error(2);
    }

    // Check the dimensions provided are valid and set the new dimensions.
    if (argc == 5) {
        int* dimension = validate_dimensions(argv[3], argv[4]);

        if (dimension == NULL) {
            return error(3);
        }
    
        dimensions[0] = dimension[0];
        dimensions[1] = dimension[1];
    }

    // Check the file input is formatted correctly and set new dimensions.
    if (argc == 4) {
        FILE* file = fopen(argv[3], "r");
        if (file == NULL) {
            return error(4);
        }

        int* dimension = validate_file(file);
        if (dimension == NULL) {
            return error(5);
        }

        dimensions[0] = dimension[0];
        dimensions[1] = dimension[1];
    }

    return 0;
}
