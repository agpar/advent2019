#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define part1 0

int *loadInput(const char* inputPath, int *sizeOut) {
    FILE *fp = fopen(inputPath, "r");

    // Find the length of the array in the file
    char c;
    int commaCount = 0;
    while((c = fgetc(fp)) != EOF) {
        if (c == ',')
            commaCount++;
    }
    *sizeOut = commaCount + 1;

    // Read in all the ints;
    fseek(fp, 0, SEEK_SET);
    int *program = malloc(sizeof(int) * (*sizeOut));
    char wordBuffer[100];
    for(int i = 0; i < *sizeOut; i++) {
        for (int j = 0; 1; j++) {
            c = fgetc(fp);
            if (c < '0' || c > '9') {
                wordBuffer[j] = '\0';
                break;
            }
            wordBuffer[j] = c;
        }
        program[i] = strtol(wordBuffer, NULL, 10);
    }
    fclose(fp);
    return program;
}

void printBounds(int *program, int programSize) {
    printf("first = %d, %d, %d last = %d, %d, %d", 
            program[0], program[1], program[2],
            program[programSize - 3], program[programSize - 2], 
            program[programSize - 1]);
}

void runProgram(int *program, int programSize) {
    for(int pc = 0; pc < programSize; pc += 4) {
        int opCode = program[pc];
        int largIdx = program[pc+1];
        int rargIdx = program[pc+2];
        int destIdx = program[pc+3];

        switch(opCode) {
            case 1:
                program[destIdx] = program[largIdx] + program[rargIdx];
                break;
            case 2:
                program[destIdx] = program[largIdx] * program[rargIdx];
                break;
            case 99:
                return;
            default:
                printf("Unknown upcode %d at pc %d\n", opCode, pc);
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Input argument required.");
        exit(1);
    }

    int programSize;
    int* program = loadInput(argv[1], &programSize);

    if (part1) {
        program[1] = 12;
        program[2] = 2;
        runProgram(program, programSize);
        printf("Answer: %d\n", program[0]);
    } else {
        int* savedProgram = malloc(sizeof(int) * programSize);
        memcpy(savedProgram, program, sizeof(int) * programSize);
        int searchValue = 19690720;

        for(int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                memcpy(program, savedProgram, sizeof(int) * programSize);
                program[1] = i;
                program[2] = j;
                runProgram(program, programSize);
                if (program[0] == searchValue) {
                    printf("Search value found at %d, %d\n", i, j);
                    printf("100 * %d + %d = %d\n", i, j, 100*i + j);
                    return 0;
                }
            }
        }
    }
    
}
