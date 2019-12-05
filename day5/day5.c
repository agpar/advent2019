#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define part1 0

#define INSTR_LEN 5

#define OP_ADD 1
#define OP_MUL 2
#define OP_BRK 99
#define OP_INP 3
#define OP_OUT 4
#define OP_JMPT 5
#define OP_JMPF 6
#define OP_BLT 7
#define OP_BEQ 8

#define MODE_POS 0
#define MODE_IMM 1

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
            if ((c < '0' || c > '9') && c != '-' && c != '+') {
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

int parseOpCode(int instruction) {
    return instruction % 100;
}

int parseMode(int instruction, int argIdx) {
    char instructionStr[INSTR_LEN + 1];
    sprintf(instructionStr, "%d", instruction); 
    int instructionLen = strlen(instructionStr);
    for(int i = 3; i <= instructionLen; i++) {
        if (i - 3 == argIdx) {
            return instructionStr[instructionLen - i] == '1' ? MODE_IMM : MODE_POS;
        }
    }
    return MODE_POS;
}

int opLen(int opCode) {
    switch(opCode) {
        case OP_BLT:
        case OP_BEQ:
        case OP_ADD:
        case OP_MUL:
            return 4;
        case OP_JMPT:
        case OP_JMPF:
            return 3;
        case OP_INP:
        case OP_OUT:
            return 2;
        case OP_BRK:
            return 1;
        default:
            printf("Unknown op code %d\n", opCode);
            exit(1);
    }
}

int getValue(int *program, int instruction, int arg, int argIdx) {
    int mode = parseMode(instruction, argIdx);
    if (mode == MODE_IMM) {
        return arg;
    } else {
        return program[arg];
    }
}


void runProgram(int *program, int programSize) {
    int pcIncr = 0;
    int args[4];
    int vals[4];

    for(int pc = 0; pc < programSize; pc += pcIncr ) {
        int instruction = program[pc];
        int opCode = parseOpCode(instruction);
        pcIncr = opLen(opCode);
        int argLen = pcIncr - 1;

        // Load the appopriate number of args for this op; 
        for(int j = 0; j < argLen; j++) {
            args[j] = program[pc+j+1];
        }

        // Load the appropriate values for each arg;
        for(int j = 0; j < argLen; j++) {
            vals[j] = getValue(program, instruction, args[j], j);
        }

        switch(opCode) {
            case OP_ADD:
                program[args[2]]= vals[0] + vals[1];
                break;
            case OP_MUL:
                program[args[2]] = vals[0] * vals[1];
                break;
            case OP_JMPT:
                if (vals[0] != 0) {
                    pcIncr = 0;
                    pc = vals[1];
                }
                break;
            case OP_JMPF:
                if (vals[0] == 0) {
                    pcIncr = 0;
                    pc = vals[1];
                }
                break;
            case OP_BLT:
                program[args[2]] = vals[0] < vals[1] ? 1 : 0;
                break;
            case OP_BEQ:
                program[args[2]] = vals[0] == vals[1] ? 1 : 0;
                break;
            case OP_BRK:
                return;
            case OP_OUT:
                printf("%d\n", vals[0]);
                break;
            case OP_INP:
                scanf("%d", &program[args[0]]);
                break;
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
    runProgram(program, programSize);
}
