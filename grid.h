typedef enum {
    EMPTY,
    PLAYER1,
    PLAYER2
} Marker;

typedef struct Grid {
    int rows;
    int columns;
    Marker** values;

    void (*set)(struct Grid*, int, int, Marker);
    Marker (*get)(struct Grid*, int, int);
    int** (*findNeighbours)(struct Grid*, int, int);
    int** (*search)(struct Grid*, int, int);
    void (*save)(struct Grid*, FILE* file);
} Grid;

Grid* create_grid(int rows, int columns);

void print_grid(Grid* grid);

Grid* load_grid(FILE* file, int rows, int columns);
