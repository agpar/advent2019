#include <stdlib.h>
#include <stdio.h>

#define part1 0
int rangeLow = 156218;
int rangeHi = 652527;
char buff[100];

char *toStr(int input) {
    sprintf(buff, "%d", input);
    return buff;
}

int hasRepeat(char *str) {
    if (str == NULL || str[0] == '\0')
        return 0;
    for(int i = 1; str[i] != '\0'; i++) {
        if (str[i-1] == str[i])
            return 1;
    }
    return 0;
}

int increasing(char *str) {
    if (str == NULL || str[0] == '\0')
        return 0;
    for (int i=1; str[i] != '\0'; i++) {
        if (str[i-1] > str[i])
            return 0;
    }
    return 1;
}

int hasDouble(char *str) {
    if (str == NULL || str[0] == '\0')
        return 0;
    char current = str[0];
    int count = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if (str[i] == current) {
            count++;
            continue;
        } else {
            if (count == 2) {
                break;
            }
            current = str[i];
            count = 1;
            
        }
    }
    return count == 2;
}

int respectsRules(int i) {
    char *strRepr = toStr(i);
    
    if (part1 && hasRepeat(strRepr) && increasing(strRepr)) {
        return 1;
    }
    if ((!part1) && hasDouble(strRepr) && increasing(strRepr)) {
        return 1;
    }
    return 0;
}

int countPasswords() {
    int passCount = 0;
    for(int i = rangeLow; i <= rangeHi; i++) {
        if (respectsRules(i)) {
            passCount++;
        }
    }
    return passCount;
}

int main() {
    printf("Found %d\n", countPasswords());
}
