#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "points_and_lines.h"

int lineMin(Line *line, int axis) {
    switch(axis) {
        case X_AXIS:
            return fmin(line->start->x, line->end->x);
        case Y_AXIS:
            return fmin(line->start->y, line->end->y);
        default:
            printf("Unknown axis");
            exit(1);
    }
}

int lineMax(Line *line, int axis) {
    switch(axis) {
        case X_AXIS:
            return fmax(line->start->x, line->end->x);
        case Y_AXIS:
            return fmax(line->start->y, line->end->y);
        default:
            printf("Unknown axis");
            exit(1);
    }
}

int lineDeltaAxis(Line *line) {
    return line->start->x == line->end->x ? Y_AXIS : X_AXIS;
}

Point *makePoint(int x, int y) {
    Point *p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

Line *makeLineFromPoints(Point *start, Point *end) {
    Line *l = malloc(sizeof(Line));
    l->start = start;
    l->end = end;
    return l;
}

Line *makeLineFromDir(Point *start, char *direction) {
    int steps = strtol(direction+1, NULL, 10);
    switch(direction[0]) {
        case 'U':
            return makeLineFromPoints(start, makePoint(start->x, start->y + steps));
        case 'D':
            return makeLineFromPoints(start, makePoint(start->x, start->y - steps));
        case 'R':
            return makeLineFromPoints(start, makePoint(start->x + steps, start->y));
        case 'L':
            return makeLineFromPoints(start, makePoint(start->x - steps, start->y));
        default:
            printf("Unkown direction %c, exiting.\n", direction[0]);
            exit(1);
    }
}
