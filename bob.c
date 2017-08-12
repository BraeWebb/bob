#include <stdio.h>
#include <stdlib.h>

#include "grid.h"
#include "validate.h"

int main(int argc, char** argv){

    int error_code = validate(argc, argv);
    if(error_code != 0){
        return error_code;
    }

    Grid* grid;

    if(argc == 5){
        char* p;
        long rows = strtol(argv[3], &p, 10);
        long columns = strtol(argv[4], &p, 10);
        grid = createGrid(rows, columns);
    } else {
        grid = createGrid(10, 7);
    }

    printGrid(grid);

    return 0;
}
