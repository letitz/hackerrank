/*
 * Simple Fenwick tree implementation
 * Additive and multiplicative modulo variants
 * Author: Titouan Rigoudy
 */

long *fenwick_make(size_t n);

long *fenwick_prodmake(size_t n);

// Add val to element i in fenwick tree
void fenwick_update(long *fenwick, size_t n, size_t i, long val);

// Sum of elements in [0, i[
long fenwick_prefixsum(long *fenwick, size_t n, size_t i);

// Sum of elements in [i, j[
long fenwick_intervalsum(long *fenwick, size_t n, size_t i, size_t j);

// Multiply element i by val in fenwick tree of products
void fenwick_produpdate(long *fenwick, size_t n, size_t i, long val, long mod);

// Product of elements in [0, i[
long fenwick_prefixprod(long *fenwick, size_t n, size_t i, long mod);

// Product of elements in [i, j[
long fenwick_intervalprod(long *fen, size_t n, size_t i, size_t j, long mod);

