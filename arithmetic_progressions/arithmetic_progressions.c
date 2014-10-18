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

int read_adp(int n, int (*adp)[3]) {
    char buf[BUF_SIZE];
    int *ints;
    size_t ints_len;

    int i;
    for (i = 0; i < n; i++) {
        fgets(buf, BUF_SIZE, stdin);
        ints_len = read_ints(buf, &ints);
        if (ints_len != 3) {
            return 1;
        }
        adp[i][0] = ints[0];
        adp[i][1] = ints[1];
        adp[i][2] = ints[2];
        free(ints);
    }
    return 0;
}

int min_const_diff(int n, int (*adp)[3], int i, int j) {
    return 0;
}

int add_powers(int n, int (*adp)[3], int i, int j, int v) {
    return 0;
}

int handle_query(char *str, int n, int (*adp)[3]) {
    int *ints;
    int ints_len = read_ints(str, &ints);
    if (ints_len < 1) {
        return 1;
    }
    int query_type = ints[0];
    if (query_type == 0) {
        if (ints_len != 3) {
            return 1;
        }
        return min_const_diff(n, adp, ints[1], ints[2]);
    }
    if (query_type == 1) {
        if (ints_len != 4) {
            return 1;
        }
        return add_powers(n, adp, ints[1], ints[2], ints[3]);
    }
    return 1;
}

int main(int argc, char **argv) {
    char buf[BUF_SIZE];
    fgets(buf, BUF_SIZE, stdin);

    char *end;
    int n = strtol(buf, &end, 10);
    if (!isspace(*end) || n < 1) {
        return 0;
    }

    int (*adp)[3] = malloc(3 * n * sizeof **adp);
    if (adp == NULL) {
        return 0;
    }
    int err = read_adp(n, adp);
    if (err) {
        return 0;
    }
    int i;
    for (i = 0; i < n; i++) {
        printf("%d %d %d\n", adp[i][0], adp[i][1], adp[i][2]);
    }

    fgets(buf, BUF_SIZE, stdin);
    int q = strtol(buf, &end, 10);
    if (!isspace(*end)) {
        return 0;
    }

    for (i = 0; i < q; i++) {
        fgets(buf, BUF_SIZE, stdin);
        err = handle_query(buf, n, adp);
        if (err) {
            return 0;
        }
    }

    return 0;
}