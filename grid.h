#define p1 1
#define p2 2

typedef struct grid {
    int rows;
    int columns;
    int** values;

    void (*set)(struct grid*, int, int, int);
    int (*get)(struct grid*, int, int);
} Grid;

Grid* createGrid(int rows, int columns);

void printGrid(Grid* grid);
