#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "day6.h"

int hash(char *key) {
    unsigned int result = 1315423911;
    for (int i = 0; key[i] != '\0'; i++) {
        result ^= ((result << 5) + (key[i]) + (result >> 2));
    }
    return result;
}

int childInList(char *child, OrbitList *list) {
    while(list != NULL) {
        if(strcmp(list->child, child) == 0)
            return 1;
        list = list->next;
    }
    return 0;
}

int listSize(OrbitList *list) {
    int size = 0;
    while(list != NULL) {
        size++;
        list = list->next;
    }
    return size;
}

HashedOrbits **makeTable(int size) {
    HashedOrbits **table = malloc(sizeof(HashedOrbits*) * size);
    for(int i = 0; i < size; i++) {
        table[i] = NULL;
    }
    return table;
}

OrbitList *getValue(char *key, HashedOrbits **table) {
    int idx = hash(key) % TABLE_SIZE;
    HashedOrbits *iter = table[idx];
    while(iter != NULL) {
        if (strcmp(iter->parent, key) == 0)
            return iter->orbits;
        iter = iter->next;
    }
    return NULL;
}

void addValue(char *parent, char *child, HashedOrbits **table) {
    OrbitList *list = getValue(parent, table);
    if (list != NULL && !childInList(child, list)) {
        OrbitList *newNode = malloc(sizeof(OrbitList));
        strcpy(newNode->child, child);
        newNode->next = list->next;
        list->next = newNode;
        return;
    }

    list = malloc(sizeof(OrbitList));
    strcpy(list->child, child);
    list->next = NULL;

    HashedOrbits *newHash = malloc(sizeof(HashedOrbits));
    strcpy(newHash->parent, parent);
    newHash->next = NULL;
    newHash->orbits = list;

    int idx = hash(parent) % TABLE_SIZE;
    if (table[idx] == NULL) {
        table[idx] = newHash;
    } else {
        newHash->next = table[idx];
        table[idx] = newHash;
    }
}

int countOrbit(HashedOrbits **table, char *parent, int depth) {
    OrbitList *directOrbits = getValue(parent, table);
    int directOrbitCount = listSize(directOrbits);
    
    int childOrbitCounts = 0;
    while (directOrbits != NULL) {
        childOrbitCounts += countOrbit(table, directOrbits->child, depth + 1);
        directOrbits = directOrbits->next;
    }

    return depth + childOrbitCounts;
}

HashedOrbits **loadValues(char *file) {
    FILE *fp = fopen(file, "r");
    char lineBuff[100];
    char *parent;
    char *child;
    HashedOrbits **table = makeTable(TABLE_SIZE);
    while(fgets(lineBuff, 100, fp) != NULL) {
        lineBuff[3] = '\0';
        lineBuff[7] = '\0';
        parent = lineBuff;
        child = lineBuff + 4;
        addValue(parent, child, table);
    }
    return table;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("At least 1 input argument is required");
        exit(1);
    }
    HashedOrbits **orbits = loadValues(argv[1]);
    printf("Count: %d\n", countOrbit(orbits, "COM", 0));
    return 0;
}
