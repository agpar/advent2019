#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RECURSIVE 1

int readNextInt(FILE *fp, int *val) {
    char lineBuffer[1000];
    if(fgets(lineBuffer, 1000, fp) == NULL)
        return -1;
    *val = strtol(lineBuffer, NULL, 10);
    return 0;
}


int calcFuel(int mass) {
    int fuelForCurrent = floorl(mass / 3.0) - 2;
    if (fuelForCurrent < 1)
        return 0;
    if (RECURSIVE) {
        return fuelForCurrent + calcFuel(fuelForCurrent);
    }
    else {
        return fuelForCurrent;
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("At least one input argument is required");
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    int nextInt;
    int sum = 0;
    while(readNextInt(fp, &nextInt) != -1) {
        sum += calcFuel(nextInt);
    }
    printf("Total fuel required: %d\n", sum);
    fclose(fp);
}
