// https://www.hackerrank.com/challenges/arithmetic-progressions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INTS_START_SIZE 8
#define INTS_DELIM " \t\n"
#define BUF_SIZE 1024
#define BIG_MOD 1000003

struct adp {
    long a;
    long d;
    long p;
    long factor;
};

// Add val to element i in fenwick tree
void fenwick_update(long *fenwick, size_t n, size_t i, long val) {
    i++;
    if (i < 1 || i > n) {
        return;
    }
    while (i <= n) {
        fenwick[i-1] += val;
        i += i & -i; // parent
    }
}

// Sum of elements in [0, i[
long fenwick_prefixsum(long *fenwick, size_t n, size_t i) {
    long sum = 0;
    while (i > 0) {
        sum += fenwick[i-1];
        i -= i & -i; // predecessor
    }
    return sum;
}

// Sum of elements in [i, j[
long fenwick_intervalsum(long *fenwick, size_t n, size_t i, size_t j) {
    return fenwick_prefixsum(fenwick, n, j) - fenwick_prefixsum(fenwick, n, i);
}

// Add val to entire interval [i, j[ of the fenwick tree
// Supposes the fenwick tree stores differences between elements
void fenwick_intervalupdate(
    long *fenwick,
    size_t n,
    size_t i,
    size_t j,
    long val)
{
    fenwick_update(fenwick, n, i, val);
    fenwick_update(fenwick, n, j, -val);
}

// Get the ith value from the fenwick tree
// Supposes the fenwick tree stores differences between elements
long fenwick_get(long *fenwick, size_t n, size_t i) {
    return fenwick_prefixsum(fenwick, n, i+1);
}

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
    char *end;
    long int tmp;
    char *tok = strtok(str, INTS_DELIM);
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
        tok = strtok(NULL, INTS_DELIM);
    }
    return i;
}

int read_adp(int n, struct adp *adp, long *ptree) {
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
        adp[i].a = ints[0];
        adp[i].d = ints[1];
        adp[i].p = ints[2];
        fenwick_intervalupdate(ptree, n, i, i+1, adp[i].p);
        free(ints);
    }
    return 0;
}

long pow_mod(long a, long b, long mod, long acc) {
    long powa = a % mod;
    while (b > 0) {
        if (b & 1) {
            acc = (acc * powa) % mod;
        }
        powa = (powa * powa) % mod;
        b >>= 1;
    }
    return acc;
}

long factorial_mod(long n, long mod, long acc) {
    int i;
    for (i = 2; i <= n; i++) {
        acc = (acc * i) % mod;
    }
    return acc;
}

int min_const_diff(int n, struct adp *adp, long *ptree, int i, int j) {
    if (i < 1 || j > n) {
        return 1;
    }
    long k = 0;
    long v = 1;
    long p;
    int l;
    for (l = i-1; l < j; l++) {
        p = fenwick_get(ptree, n, l);
        if (p > 0) {
            k += p;
            if (adp[l].d == 0) {
                v = pow_mod(adp[l].a, p, BIG_MOD, v);
            } else {
                v = pow_mod(adp[l].d, p, BIG_MOD, v);
            }
        }
    }
    v = factorial_mod(k, BIG_MOD, v);
    printf("%ld %ld\n", k, v);
    return 0;
}

int add_powers(int n, struct adp *adp, long *ptree, int i, int j, int v) {
    if (i < 1 || j > n) {
        return 1;
    }
    fenwick_intervalupdate(ptree, n, i-1, j, v);    
    return 0;
}

int handle_query(char *str, int n, struct adp *adp, long *ptree) {
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
        return min_const_diff(n, adp, ptree, ints[1], ints[2]);
    }
    if (query_type == 1) {
        if (ints_len != 4) {
            return 1;
        }
        return add_powers(n, adp, ptree, ints[1], ints[2], ints[3]);
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

    struct adp *adp = malloc(n * sizeof *adp);
    long *ptree = calloc(n, sizeof *ptree);
    if (adp == NULL || ptree == NULL) {
        return 0;
    }
    int err = read_adp(n, adp, ptree);
    if (err) {
        return 0;
    }

    fgets(buf, BUF_SIZE, stdin);
    int q = strtol(buf, &end, 10);
    if (!isspace(*end)) {
        return 0;
    }

    int i;
    for (i = 0; i < q; i++) {
        fgets(buf, BUF_SIZE, stdin);
        err = handle_query(buf, n, adp, ptree);
        if (err) {
            return 0;
        }
    }

    return 0;
}