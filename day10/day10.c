#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Asteroid {
    int x;
    int y;
    struct Asteroid *next;
    int passed;
    int destroyed;
} Asteroid;

typedef struct Line {
    double slope;
    double intercept;
} Line;

Asteroid *makeAsteroid(int x, int y) {
    Asteroid *newAsteroid = malloc(sizeof(Asteroid));
    newAsteroid->x = x;
    newAsteroid->y = y;
    newAsteroid->next = NULL;
    newAsteroid->passed = 0;
    newAsteroid->destroyed = 0;
    return newAsteroid;
}

Asteroid *parseInput(const char *inputFile) {
    int x, y;
    char c;
    x = y = 0;
    Asteroid *asteroids = NULL;

    FILE *fp = fopen(inputFile, "r");
    while((c = getc(fp)) != EOF) {
        if (c == '\n') {
            x = 0;
            y++;
            continue;
        }
        if (c == '#') {
            Asteroid *newAsteroid = makeAsteroid(x, y);
            if (asteroids == NULL) {
                asteroids = newAsteroid;
            } else {
                newAsteroid->next = asteroids;
                asteroids = newAsteroid;
            }
        }
        x++;
    }

    fclose(fp);
    return asteroids;
}

// Assumes asteroids are not in a vertical line.
Line *computeIntersectingLine(Asteroid *a1, Asteroid *a2, Line *lineOut) {
    if (a1->x == a2->x) {
        printf("ERROR: Cant' compute a line between vertically aligned asteroids");
        exit(1);
    }

    Asteroid *minx, *maxx;
    if(a1->x < a2->x) {
        minx = a1;
        maxx = a2;
    } else {
        minx = a2;
        maxx = a1;
    }

    lineOut->slope = ((double) maxx->y - minx->y) / ((double) maxx->x - minx->x);
    lineOut->intercept = minx->y - (minx->x * lineOut->slope);
    return lineOut;
}

int doesIntersect(Asteroid *a, Line *line) {
    return fabs(line->slope * a->x + line->intercept - a->y) < 0.0000001;
}

int verticallyAligned(Asteroid *a1, Asteroid *a2) {
    return a1->x == a2->x;
}

double dist(Asteroid *a1, Asteroid *a2) {
    return sqrt(pow(a1->x - a2->x, 2) + pow(a1->y - a2->y, 2));
}

double sameSideOfA1(Asteroid *a1, Asteroid *a2, Asteroid *a3) {
    if ((a2->x < a1->x && a3->x > a1->x) ||
        (a2->x > a1->x && a3->x < a1->x))
        return 0;
    return 1;
}

// Assumes a1 and a2 are NOT vertically aligned.
// Assumes a1 and a2 are NOT the same asteroid.
int hasLignOfSite(Asteroid *a1, Asteroid *a2, Asteroid *others) {
    Line line;
    computeIntersectingLine(a1, a2, &line);
    Asteroid *a3 = others;
    while(a3 != NULL) {
        if (doesIntersect(a3, &line) && 
                dist(a1, a3) != 0 && 
                dist(a2, a3) != 0 &&
                sameSideOfA1(a1, a2,a3) &&
                dist(a1, a3) < dist(a1, a2)) {
            return 0;
        }
        a3 = a3->next;
    }
    return 1;
}

int countIntersections(Asteroid *a1, Asteroid *all) {
    int count = 0;
    int foundVertAbove = 0;
    int foundVertBelow = 0;
    Asteroid *a2 = all;
    Line line;

    while(a2 != NULL) {
        if (a1->x == a2->x && a1->y == a2->y) {
            a2 = a2->next;
            continue;
        }

        if (verticallyAligned(a1, a2)) {
            if (a1->y < a2->y && !foundVertAbove) {
                count++;
                foundVertAbove = 1;
            }
            if (a1->y > a2->y && !foundVertBelow) {
                count++;
                foundVertBelow = 1;
            }
        } else {
            if (hasLignOfSite(a1, a2, all)) {
                count++;
            }
        }
        a2 = a2->next;
    }
    return count;
}

// Compute the angle between base and other, relative to a vertical
// axis eminating from base. This was tough.
double relativeAngle(Asteroid *base, Asteroid *other) {
    int relX = other->x - base->x;
    // y is flipped here to be upwards facing.
    int relY = base->y - other->y;
    double len = sqrt(pow(relX, 2) + pow(relY, 2));
    
    double normX = (double) relX / len;
    double angle = acos(normX / 1.0);

    // Get the "full" angle, as acos only covers half the circle
    int yPos = relY >= 0 ? 1 : 0;
    if (!yPos)
        angle = 2 * M_PI - angle;

    // Now reverse the direction of travel;
    angle = (2 * M_PI) - angle;

    // Now rotate everything by pi/2, making straight up be the basis for angles.
    angle = angle + M_PI / 2.0;
    if (angle >= 2 * M_PI) {
        angle = angle - 2 * M_PI;
    }

    return angle;
}

Asteroid *part1(Asteroid *asteroids) {
    int mostIntersections = 0;
    Asteroid *bestAsteroid = NULL;
    Asteroid *a = asteroids;

    while(a != NULL) {
        int intersections = countIntersections(a, asteroids);
        if (intersections > mostIntersections) {
            mostIntersections = intersections;
            bestAsteroid = a;
        }
        a = a->next;
    }

    printf("Best asteroid is at %d, %d with %d intersections\n",
            bestAsteroid->x, bestAsteroid->y, mostIntersections);
    return bestAsteroid;
}

Asteroid *closestRotational(Asteroid *base, Asteroid *asteroids) {
    // Find the smallest angle change to a fresh asteroid
    double smallestAngle = INFINITY;
    Asteroid *other = asteroids;
    while (other != NULL) {
        if (!other->destroyed && !other->passed) {
            double angle = relativeAngle(base, other);
            smallestAngle = fmin(smallestAngle, angle);
        }
        other = other->next;
    }

    // Find the closest active asteroid at this rotational distance
    other = asteroids;
    double closestDist = INFINITY;
    Asteroid *closestActive = NULL;
    while (other != NULL) {
        if(!other->destroyed && !other->passed) {
            if(fabs(relativeAngle(base, other) - smallestAngle) < 0.00001) {
                if (dist(base, other) < closestDist) {
                    closestDist = dist(base, other);
                    if (closestActive != NULL)
                        closestActive->passed = 1;
                    closestActive = other;
                } else {
                    other->passed = 1;
                }
            }
        }

        other = other->next;
    }

    return closestActive;
}

int someNotDestroyed(Asteroid *asteroids) {
    while(asteroids != NULL) {
        if (!asteroids->destroyed)
            return 1;
        asteroids = asteroids->next;
    }
}

void part2(Asteroid *base, Asteroid *asteroids) {
    base->destroyed = 1;

    Asteroid *closest;
    int destroyedCount = 0;
    while(someNotDestroyed(asteroids)) {
        while ((closest = closestRotational(base, asteroids)) != NULL) {
            closest->destroyed = 1;
            destroyedCount++;
            printf("%d: Asteroid (%d, %d)\n", destroyedCount, closest->x, closest->y);
        }
        Asteroid *resetter = asteroids;
        while(resetter != NULL) {
            resetter->passed = 0;
            resetter = resetter->next;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s {input-file}\n", argv[0]);
        exit(1);
    }
    Asteroid *asteroids = parseInput(argv[1]);
    Asteroid *base = part1(asteroids);
    part2(base, asteroids);

}
