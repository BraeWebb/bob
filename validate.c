#include <stdio.h>
#include <string.h>

#include "validate.h"

#define USAGE "Usage: bob p1type p2type [height width | filename]\n"
#define INVALID_TYPE "Invalid type\n"
#define GRID_DIMENSIONS "Sensible board dimensions please!"
#define FILE_OPEN "Could not start reading from savefile"
#define FILE_READ "Incorrect file contents"

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
        printf(GRID_DIMENSIONS);
        return 3;
    }

    if(argc == 4){
        printf(FILE_OPEN);
        return 4;
    }

    return 0;
}
