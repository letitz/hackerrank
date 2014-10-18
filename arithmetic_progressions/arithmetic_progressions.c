#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INTS_START_SIZE 8
#define INTS_DELIM " \t\n"
#define BUF_SIZE 1024

size_t read_ints(char *str, int **ints) {
    if (str == NULL) {
        return 0;
    }
    size_t size = INTS_START_SIZE;
    *ints = malloc(INTS_START_SIZE * sizeof *ints);
    if (*ints == NULL) {
        return 0;
    }

    size_t i = 0;
    char *toksave, *end;
    long int tmp;
    char *tok = strtok_r(str, INTS_DELIM, &toksave);
    while (tok != NULL) {
        tmp = strtol(tok, &end, 10);
        if (*end == '\0' || isspace(*end)) { // valid number found
            if (i == size) {
                // If we have filled array, double its size
                size *= 2;
                *ints = realloc(*ints, size * sizeof *ints);
                if (*ints == NULL) {
                    return 0;
                }
            }
            // store in array
            (*ints)[i] = tmp;
            i++;
        }
        tok = strtok_r(NULL, INTS_DELIM, &toksave);
    }
    return i;
}

int main(int argc, char **argv) {
    char buf[BUF_SIZE];
    fgets(buf, BUF_SIZE, stdin);
    int *ints;
    size_t numints =  read_ints(buf, &ints);
    int i;
    for (i = 0; i < numints; i++) {
        printf("%d\n", ints[i]);
    }
    return 0;
}