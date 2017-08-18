#include <stdlib.h>
#include <stdio.h>

#include "grid.h"

/**
 * Takes a marker enum and converts that marker into its character
 * representation.
 */
char convert_character(Marker marker) {
    char markers[3] = {'.', 'O', 'X'};
    
    return markers[marker];
}

/**
 * Takes a Grid structure pointer, an x and y coordinate and Marker.
 *
 * Sets the value of the position x,y on grid to the Marker value.
 */
void set_grid_value(Grid* grid, int x, int y, Marker value) {
    grid->values[x][y] = value;
}

/**
 * Returns the Marker value at the position x, y on the grid.
 */
Marker get_grid_value(Grid* grid, int x, int y) {
    return grid->values[x][y];
}

/**
 * Determines if an x and y coordinate is contained within a given grid.
 */
int is_in_grid(Grid* grid, int x, int y) {
    if (x >= 0 && y >= 0) {
        if (x < grid->rows && y < grid->columns) {
            return 1;
        }
    }
    return 0;
}

/**
 * Returns all of the valid neighbouring positions to the given x and y
 * coordinate on the grid.
 *
 * Returns a pointer to an array of 6 values, each value is a pointer to a two
 * value (x, y) array or NULL if that position is invalid.
 */
int** get_neighbours(Grid* grid, int x, int y) {
    
    // Allocate space for the 6 possible neighbours.
    int** neighbours = malloc(sizeof(int*) * 6);

    int index = 0;
    
    // Look through the 6 possible neighbours.
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (abs(i - j) <= 1 && !(i == 0 && j == 0)) {
                
                int neighbourX = x + i;
                int neighbourY = y + j;
                
                // If that position is in the grid then add it to the
                // neighbours array.
                if (is_in_grid(grid, neighbourX, neighbourY)) {
                    int* position = malloc(sizeof(int) * 2);
                    
                    position[0] = neighbourX;
                    position[1] = neighbourY;
                    
                    neighbours[index] = position;
                } else {
                    // If the position is invalid then set it to NULL.
                    neighbours[index] = NULL;
                }

                index++;
            }
        }
    }

    return neighbours;
}

/**
 * Takes an array, position, of length 2 representing x and y. Also takes
 * an array of arrays of length size.
 *
 * Returns 1 iff the position is inside the array, otherwise 0.
 */
int position_in_array(int* position, int** array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] != NULL) {
            if (array[i][0] == position[0] && array[i][1] == position[1]) {
                return 1;
            }
        }   
    }
    return 0;
}


/**
 * Look for all positions connected to the startX, startY position.
 *
 * A position is considered connected iff there exists a path of neighbours
 * which are of the same Marker type from the start position to the end 
 * position.
 *
 * Returns an array of position arrays.
 *
 * The length of the returned array will be the grid rows * the grid columns.
 * A position array is an array of length 2 which holds an x and y value.
 */
int** search_grid(Grid* grid, int startX, int startY) {
    // Theoretically it shouldn't need to search more than half the grid...
    // but just to be safe
    int maxSize = grid->rows * grid->columns;
    int** nodes = malloc(sizeof(int*) * maxSize);
    int** visited = malloc(sizeof(int*) * maxSize);
    
    // Set the array values to be zero.
    for (int i = 0; i < maxSize; i++) {
        nodes[i] = NULL;
        visited[i] = NULL;
    }

    int type = grid->get(grid, startX, startY);
    
    nodes[0] = malloc(sizeof(int) * 2);
    nodes[0][0] = startX;
    nodes[0][1] = startY;
    
    int nodeCount = 1;
    int visitedCount = 0;

    // Depth first search through all the positions it can find.
    while (nodeCount > 0) {
        // Pop off the next node in the list.
        int* node = nodes[--nodeCount];
        
        int x = node[0];
        int y = node[1];
        
        if (grid->get(grid, x, y) == type) {
            // Ensure this node hasn't been visited already.
            if (!position_in_array(node, visited, maxSize)) {
                
                // Find all of the nodes neighbours.
                int** neighbours = grid->findNeighbours(grid, x, y);
                
                // Add all valid neighbours to the node search.
                for (int i = 0; i < 6; i++) {
                    if (neighbours[i] != NULL) {
                        nodes[nodeCount++] = neighbours[i];
                    }
                }
                
                visited[visitedCount++] = node;
            }
        }
    }

    return visited;
}


/**
 * Exports the state of the current grid to an output file.
 */
void serialize_grid(Grid* grid, FILE* output) {

    for (int x = 0; x < grid->rows; x++) {
        char* row = malloc(sizeof(char) * grid->columns);

        for (int y = 0; y < grid->columns; y++) {
            row[y] = convert_character(grid->get(grid, x, y));
        }

        fprintf(output, row);
        fprintf(output, "\n");
    }

}


/**
 * Takes a grid and sets the structure properties to appropriate functions.
 */
Grid* create_grid_methods(Grid* grid) {
    grid->set = set_grid_value;
    grid->get = get_grid_value;
    grid->findNeighbours = get_neighbours;
    grid->search = search_grid;
    grid->save = serialize_grid;

    return grid;
}

/**
 * Creates a grid structure based off the amount of rows and columns in a grid.
 *
 * Returns a pointer to the newly created grid structure.
 */
Grid* create_grid(int rows, int columns) {
    Marker** values = malloc(sizeof(Marker*) * rows);
    
    // Allocate space for each row.
    for (int i = 0; i < rows; i++) {
        values[i] = malloc(sizeof(Marker) * columns);
    }
    
    Grid* grid = malloc(sizeof(Grid));
    
    // Assign properties
    grid->rows = rows;
    grid->columns = columns;
    grid->values = values;

    // Assign methods
    grid = create_grid_methods(grid);

    return grid;
}

/**
 * Prints out the grid based on the grids current state.
 */
void print_grid(Grid* grid) {
    // Loops through each row in the grid.
    for (int x = 0; x < grid->rows; x++) {
        // Add indentation to the start of the row.
        for (int space = 1; space < grid->rows - x; space++) {
            printf(" ");
        }
        // Print out every character.
        for (int y = 0; y < grid->columns; y++) {
            if (y != 0) {
                printf(" ");
            }
            printf("%c", convert_character(grid->values[x][y]));
        }
        printf("\n");
    }
}

/**
 * Reads the contents of a file to fill out a grid's values array.
 */
void read_file(FILE* file, Marker** values) {
    int started = 0;
    int x = 0;
    int y = 0;
    Marker marker;    
    int next = 0;

    while (1) {
        next = fgetc(file);
        // Stop reading at the end of the file.
        if (next == EOF) {
            break;
        }

        // Move onto the next row on a new line.
        if (next == '\n') {
            if (!started) {
                started = 1;
            } else {
                x++;
                y = 0;
            }
            continue;
        }

        // Only run once the first line has been passed.
        if (!started) {
            continue;
        }

        // Assign the marker value based on the character.
        if (next == 'O') {
            marker = PLAYER1;
        } else if (next == 'X') {
            marker = PLAYER2;
        } else if (next == '.') {
            marker = EMPTY;
        }

        values[x][y] = marker;
        y++;
    }
}


/**
 * Reads through a file and creates a grid based on the file.
 *
 * Returns a pointer to a new grid with the given rows, columns and values
 * assigned based off the file contents.
 */
Grid* load_grid(FILE* file, int rows, int columns) {

    // Create a new empty grid.
    Grid* grid = create_grid(rows, columns);
    
    Marker** values = grid->values;

    read_file(file, values);
    
    return grid;
}
