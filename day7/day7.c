#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "int_comp.h"

#define AMP_NUM 5
#define PHASE_MAX 4
#define PHASE_MIN 0
#define INPUT_VAL 0

#define PART1 0

void swap(int *arr, int i, int j) {
    int buf = arr[i];
    arr[i] = arr[j];
    arr[j] = buf;
}

int *permute(int *phases, int l, int r, int *steps) {  
    int buf;
    int *res;
    if (l == r) {
        if (*steps == 0) {
            return phases;
        } else {
            *steps = *steps - 1;
            return NULL;
        }
    }
    else
    {  
        for (int i = l; i <= r; i++)  
        {  
            swap(phases, l, i);
            res = permute(phases, l+1, r, steps);  
            if (res != NULL) {
                return res;
            } 
            swap(phases, l, i);
        }  
        return NULL;
    }  
}  

int *nextPhaseSetting1(int *phases, int steps) {
    for(int i = 0; i < AMP_NUM; i++) {
        phases[i] = i;
    }
    int localSteps = steps;

    return permute(phases, 0, AMP_NUM - 1, &localSteps);
}

int *nextPhaseSetting2(int *phases, int steps) {
    for(int i = 0; i < AMP_NUM; i++) {
        phases[i] = i + 5;
    }
    int localSteps = steps;

    return permute(phases, 0, AMP_NUM - 1, &localSteps);
}

Program *copyProgram(Program *program) {
    int *tapeCopy = malloc(sizeof(int)* program->size);
    memcpy(tapeCopy, program->tape, sizeof(int) * program->size);
    Program *progCopy = malloc(sizeof(Program));
    progCopy->tape = tapeCopy;
    progCopy->pc = program->pc;
    progCopy->size = program->size;
    return progCopy;
}

void freeProgram(Program *program) {
    free(program->tape);
    free(program);
}

int runProgramThroughAmps1(Program *program, int *phases) {
    Program *programCopy = copyProgram(program);
    IO out;
    out.value = 0;
    out.state = STATE_WRITE;
    IO in;
    in.value = 0;
    in.state = STATE_READ;

    for(int i = 0; i < AMP_NUM; i++) {
        // Load a new copy of the program.
        freeProgram(programCopy);
        programCopy = copyProgram(program);

        // Load values into input and run;
        in.value = phases[i];
        in.state = STATE_READ;
        runProgram(programCopy, &in, &out);

        in.value = out.value;
        in.state = STATE_READ;
        out.state = STATE_WRITE;
        runProgram(programCopy, &in, &out);
    }

    return out.value;
}

int runProgramThroughAmps2(Program *program, int *phases) {
    Program **programCopies = malloc(sizeof(Program *) * AMP_NUM);
    for (int i = 0; i < AMP_NUM; i++) {
        programCopies[i] = copyProgram(program);
    }

    // Load phase values into pipes
    IO pipes[AMP_NUM];
    for(int i = 0; i < AMP_NUM; i++) {
        pipes[i].state = STATE_READ;
        pipes[i].value = phases[i];
    }
    
    // Start the first program going with phase and 0 input.
    runProgram(programCopies[0], &pipes[0], &pipes[1]);
    pipes[0].state = STATE_READ;
    pipes[0].value = 0;


    // Loop through programs until they all exit
    int exitCount = 0;
    int exitSig;

    while(exitCount < AMP_NUM) {
        for (int i = 0; i < AMP_NUM; i++) {
            exitSig = runProgram(programCopies[i], &pipes[i], &pipes[(i+1) % AMP_NUM]);
            if (exitSig == SIG_EXIT)
                exitCount++;
        }
        if (exitCount == AMP_NUM)
            break;
        exitCount = 0;
    }

    for (int i = 0; i < AMP_NUM; i++) {
        freeProgram(programCopies[i]);
    }

    return pipes[0].value;
}

int main(int argc, char *argv[]) {
    // Load the program
    Program *program = loadInput(argv[1]);

    // Initialize the phase array for the amplifiers
    int phases[AMP_NUM];
    for(int i = 0; i < AMP_NUM; i++)
        phases[i] = 4-i;

    int i = 0;
    int maxAmp = 0;
    if (PART1) {
        while(nextPhaseSetting1(phases, i) != NULL) {
            maxAmp = fmax(maxAmp, runProgramThroughAmps1(program, phases));
            i++;
        }
    } else {
        while(nextPhaseSetting2(phases, i) != NULL) {
            maxAmp = fmax(maxAmp, runProgramThroughAmps2(program, phases));
            i++;
        }
    }


    printf("Max amp setting: %d\n", maxAmp);
}


