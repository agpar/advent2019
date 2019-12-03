#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "points_and_lines.h"
#include "csv.h"

#define part1 0

Line *parseDirectionsToLines(char **directions) {
    Point *lastPoint = makePoint(0,0);
    Line *firstLine = makeLineFromDir(lastPoint, *directions);
    Line *lastLine = firstLine;
    directions++;

    while (*directions != NULL) {
        Line *newLine = makeLineFromDir(lastLine->end, *directions);
        lastLine->next = newLine;

        lastLine = newLine;
        directions++;
    }
    return firstLine;
}

Point *lineIntersection(Line *line1, Line *line2) {
    int line1DeltaAxis = lineDeltaAxis(line1);
    int line2DeltaAxis = lineDeltaAxis(line2);;

    if (line1DeltaAxis == line2DeltaAxis)
        return NULL;


    if (line1DeltaAxis == X_AXIS) {
        int xLower = lineMin(line1, X_AXIS);
        int xUpper = lineMax(line1, X_AXIS);
        int yThresh = line1->start->y;
        if (line2->start->x > xUpper || line2->start->x < xLower)
            return NULL;
        if (lineMax(line2, Y_AXIS) >= yThresh &&
            lineMin(line2, Y_AXIS) <= yThresh) {
            return makePoint(line2->start->x, yThresh);
        }
    }
    if (line1DeltaAxis == Y_AXIS) {
        int yLower = lineMin(line1, Y_AXIS);
        int yUpper = lineMax(line1, Y_AXIS);
        int xThresh = line1->start->x;
        if (line2->start->y > yUpper || line2->start->y < yLower)
            return NULL;
        if (lineMax(line2, X_AXIS) >= xThresh &&
            lineMin(line2, X_AXIS) <= xThresh) {
            return makePoint(xThresh, line2->start->y);
        }
    }

    return NULL;
}

int manhattenDistanceToOrigin(Point *p) {
    return abs(p->x) + abs(p->y);
}

int lineIntersectsPoint(Line *line, Point *p) {
    int deltaAxis = lineDeltaAxis(line);
    if (deltaAxis == X_AXIS && line->start->y == p->y &&
        p->x >= lineMin(line, X_AXIS) && p->x <= lineMax(line, X_AXIS)) {
        return 1;
    }
    if (deltaAxis == Y_AXIS && line->start->x == p->x &&
        p->y >= lineMin(line, Y_AXIS) && p->y <= lineMax(line, Y_AXIS)) {
        return 1;
    }
    return 0;
}

int lineDistanceToPoint(Line *line, Point *p) { 
    int dist = 0;
    while(line != NULL) {
        int deltaAxis = lineDeltaAxis(line);
        if (lineIntersectsPoint(line, p)) {
            if (deltaAxis == X_AXIS)
                dist += abs(line->start->x - p->x);
            if (deltaAxis == Y_AXIS)
                dist += abs(line->start->y - p->y);
            return dist;
        } else {
            if (deltaAxis == X_AXIS)
                dist += abs(line->start->x - line->end->x);
            if (deltaAxis == Y_AXIS)
                dist += abs(line->start->y - line->end->y);
        }

        line = line->next;
    }
    printf("Line does not intersect point... Why did you call this?");
    return 10000000;
}

void findClosestIntersection(Line *lines1, Line *lines2) {
    int closestPointDist = -1;
    Line *outerLine = lines1;
    Line *innerLine = lines2;
    int i = 0;
    while(outerLine != NULL) {
        while(innerLine != NULL) {
            Point *intersection = lineIntersection(outerLine, innerLine);
            if (intersection != NULL && !(intersection->x == 0 && intersection->y == 0)) {
                int interDist;
                if (part1) {
                    interDist = manhattenDistanceToOrigin(intersection);
                } else {
                    interDist = lineDistanceToPoint(lines1, intersection) + lineDistanceToPoint(lines2, intersection);
                }

                if (closestPointDist == -1) {
                    closestPointDist = interDist;
                } else {
                    closestPointDist = fmin(closestPointDist, interDist);
                }
            }
            innerLine = innerLine->next;
        }
        innerLine = lines2;
        outerLine = outerLine->next;
    }
    printf("Closest point is at dist: %d", closestPointDist);
}

int main(int argc, char*argv[]) {
    if (argc < 2) {
        printf("Input file argument required");
        exit(1);
    }
    FILE *fp = fopen(argv[1], "r");
    char **tokens1 = readCSVLine(fp);
    char **tokens2 = readCSVLine(fp);
    fclose(fp);

    Line *lines1 = parseDirectionsToLines(tokens1);
    Line *lines2 = parseDirectionsToLines(tokens2);
    findClosestIntersection(lines1, lines2);

}
