#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"

static char buffer[BUF_LEN];

char **readCSVLine(FILE *fp) {
    fgets(buffer, BUF_LEN, fp);
    int bufferStrlen = strlen(buffer);
    if (bufferStrlen == 0) {
        return NULL;
    }
    char *savedLine = malloc(strlen(buffer) + 1);

    // Copy buffer and count tokens;
    int i = 0;
    int tokenCount = 1;
    while (1) {
        if (buffer[i] == ',')
            tokenCount++;
        if (buffer[i] == '\n' || buffer[i] == '\0') {
            savedLine[i] == '\0';
            break;
        }
        savedLine[i] = buffer[i];
        i++;
    }

    // Tokenize the line on commas.
    char *token;
    token = strtok(savedLine, ",");
    char **tokens = malloc(sizeof(char*) * tokenCount + 1);
    tokens[0] = token;
    i = 1;
    while(token != NULL) {
        token = strtok(NULL, ",");
        tokens[i] = token;
        i++;
    }
    tokens[tokenCount] = NULL;
    return tokens;
}
