#include <stdio.h>
#include <stdlib.h>
#include "int_comp.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s {input-file}", argv[0]);
        exit(1);
    }

    Program *program = loadInput(argv[1]);
    runStdProgram(program);
}
