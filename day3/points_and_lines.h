#define X_AXIS  0
#define Y_AXIS  1

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Line {
    Point *start;
    Point *end;
    struct Line *next;
} Line;

int lineMin(Line *line, int axis);

int lineMax(Line *line, int axis);

int lineDeltaAxis(Line *line);

Point *makePoint(int x, int y);

Line *makeLineFromPoints(Point *start, Point *end);

Line *makeLineFromDir(Point *start, char *direction);
