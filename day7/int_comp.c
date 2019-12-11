#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "int_comp.h"

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

Program *loadInput(const char* inputPath) {
    FILE *fp = fopen(inputPath, "r");

    // Find the length of the array in the file
    char c;
    int programSize = 1;
    while((c = fgetc(fp)) != EOF) {
        if (c == ',')
            programSize++;
    }

    // Read in all the ints;
    fseek(fp, 0, SEEK_SET);
    int *tape = malloc(sizeof(int) * programSize);
    char wordBuffer[100];
    for(int i = 0; i < programSize; i++) {
        for (int j = 0; 1; j++) {
            c = fgetc(fp);
            if ((c < '0' || c > '9') && c != '-' && c != '+') {
                wordBuffer[j] = '\0';
                break;
            }
            wordBuffer[j] = c;
        }
        tape[i] = strtol(wordBuffer, NULL, 10);
    }
    fclose(fp);

    Program *program = malloc(sizeof(Program));
    program->tape = tape;
    program->size = programSize;
    program->pc = 0;

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

int getValue(Program *program, int instruction, int arg, int argIdx) {
    int mode = parseMode(instruction, argIdx);
    if (mode == MODE_IMM) {
        return arg;
    } else {
        return program->tape[arg];
    }
}

int runProgram(Program *program, IO *in, IO *out) {
    int pcIncr = 0;
    int args[4];
    int vals[4];

    while(program->pc < program->size) {
        int instruction = program->tape[program->pc];
        int opCode = parseOpCode(instruction);
        pcIncr = opLen(opCode);
        int argLen = pcIncr - 1;

        // Load the appopriate number of args for this op; 
        for(int j = 0; j < argLen; j++) {
            args[j] = program->tape[program->pc+j+1];
        }

        // Load the appropriate values for each arg;
        for(int j = 0; j < argLen; j++) {
            vals[j] = getValue(program, instruction, args[j], j);
        }

        switch(opCode) {
            case OP_ADD:
                program->tape[args[2]]= vals[0] + vals[1];
                break;
            case OP_MUL:
                program->tape[args[2]] = vals[0] * vals[1];
                break;
            case OP_JMPT:
                if (vals[0] != 0) {
                    pcIncr = 0;
                    program->pc = vals[1];
                }
                break;
            case OP_JMPF:
                if (vals[0] == 0) {
                    pcIncr = 0;
                    program->pc = vals[1];
                }
                break;
            case OP_BLT:
                program->tape[args[2]] = vals[0] < vals[1] ? 1 : 0;
                break;
            case OP_BEQ:
                program->tape[args[2]] = vals[0] == vals[1] ? 1 : 0;
                break;
            case OP_BRK:
                return SIG_EXIT;
            case OP_OUT:
                if (out->state == STATE_WRITE) {
                    out->value = vals[0];
                    out->state = STATE_READ;
                } else {
                    return SIG_IO;
                }
                break;
            case OP_INP:
                if (in->state == STATE_READ) {
                    program->tape[args[0]] = in->value;
                    in->state = STATE_WRITE;
                } else {
                    return SIG_IO;
                }
                break;
            default:
                printf("Unknown upcode %d at pc %d\n", opCode, program->pc);
                break;
        }
        program->pc += pcIncr;
    }
}

