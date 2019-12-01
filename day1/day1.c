#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_LEN 100
int inputData[100];

int* readInput(const char *fname, int *inputData) {
    FILE *fp = fopen(fname, "r");
    char lineBuffer[1000];
    int i = 0;
    while(fgets(lineBuffer, 1000, fp) != NULL && i < INPUT_LEN) {
        inputData[i] = strtol(lineBuffer, NULL, 10);
        printf("%d\n", inputData[i]);
        i++;
    }

    return inputData;
}

int readNextInt(FILE *fp, int *val) {
    char lineBuffer[1000];
    if(fgets(lineBuffer, 1000, fp) == NULL)
        return -1;
    *val = strtol(lineBuffer, NULL, 10);
    return 0;
}


int calcFuel(int mass) {
    return floorl(mass / 3.0) - 2;
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
}