/*
 * Author: Titouan Rigoudy
 */

#include <stdlib.h>

long *fenwick_make(size_t n) {
    long *fenwick = calloc(n, sizeof *fenwick);
    return fenwick;
}

long *fenwick_prodmake(size_t n) {
    long *fenwick = malloc(n * sizeof *fenwick);
    if (fenwick == NULL) {
        return NULL;
    }
    size_t i;
    for (i = 0; i < n; i++) {
        fenwick[i] = 1;
    }
    return fenwick;
}

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

// Multiply element i by val in fenwick tree of products
void fenwick_produpdate(long *fenwick, size_t n, size_t i, long val, long mod) {
    i++;
    if (i < 1 || i > n) {
        return;
    }
    while (i <= n) {
        fenwick[i-1] = (fenwick[i-1] * val) % mod;
        i += i & -i; // parent
    }
}

// Product of elements in [0, i[
long fenwick_prefixprod(long *fenwick, size_t n, size_t i, long mod) {
    long prod = 1;
    while (i > 0) {
        prod = (prod * fenwick[i-1]) % mod;
        i -= i & -i; // predecessor
    }
    return prod;
}

// Product of elements in [i, j[
long fenwick_intervalprod(long *fenwick, size_t n, size_t i, size_t j, long mod) {
    long p_j = fenwick_prefixprod(fenwick, n, j, mod);
    long p_i = fenwick_prefixprod(fenwick, n, i, mod);
    return (p_j * INVERSES[p_i]) % BIG_MOD;
}

