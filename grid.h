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
} Grid;

Grid* createGrid(int rows, int columns);

void printGrid(Grid* grid);
