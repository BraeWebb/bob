typedef enum {
    EMPTY,
    PLAYER1,
    PLAYER2
} Marker;

typedef struct grid {
    int rows;
    int columns;
    Marker** values;

    void (*set)(struct grid*, int, int, Marker);
    Marker (*get)(struct grid*, int, int);
    char* (*serialize)(struct grid*);
    int** (*find_neighbours)(struct grid*, int, int);
    int** (*search)(struct grid*, int, int);
} Grid;

Grid* create_grid(int rows, int columns);

void print_grid(Grid* grid);

Grid* load_grid(FILE* file, int rows, int columns);
