/*
 * Author: Titouan Rigoudy
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "segtree.h"

// Allocate new segment tree spanning the [i, j[ interval with value v
struct segtree *segtree_new(int i, int j, long v) {
    if (i < 0  || j <= i) {
        return NULL;
    }
    struct segtree *st = malloc(sizeof *st);
    if (!st) {
        return NULL;
    }
    st->i = i;
    st->j = j;
    st->v = v;
    st->left = NULL;
    st->right = NULL;
    return st;
}

struct segtree *segtree_new_simple(int n) {
    int x;
    for (x = 1; x < n; x *= 2) {}
    return segtree_new(0, x, 0);
}

// Free segment tree data structure
void segtree_free(struct segtree *st) {
    if (!st) {
        return;
    }
    segtree_free(st->left);
    segtree_free(st->right);
    free(st);
}

void segtree_print_prefix(struct segtree *st, char *prefix) {
    if (!st) {
        printf("%sNULL\n", prefix);
    }
    printf("%ssegtree{i: %d, j: %d, v: %ld}\n", prefix, st->i, st->j, st->v);
    
    int n = strlen(prefix);
    char *newprefix = malloc((n+3) * sizeof(*newprefix));
    snprintf(newprefix, n+3, "%s  ", prefix);

    if (st->left) {
        segtree_print_prefix(st->left, newprefix);
    }
    if (st->right) {
        segtree_print_prefix(st->right, newprefix);
    }
    free(newprefix);
}

// Rudimentary debug printing
void segtree_print(struct segtree *st) {
    segtree_print_prefix(st, "");
}

// Add v to the [i, j[ interval in the segment tree st
void segtree_add(struct segtree *st, int i, int j, long v) {
    if (!st) {
        fprintf(stderr, "Trying to add to null segtree\n");
        return;
    }
    if (i < st->i || j > st->j) {
        fprintf(stderr, "Trying to add out of bounds of segtree\n");
        return;
    }
    
    if (i == st->i && j == st->j) {
        st->v += v;
        return;
    }
    
    int mid = (st->i + st->j) / 2;
    
    if (!st->left) {
        st->left = segtree_new(st->i, mid, 0);
    }
    if (j <= mid) {
        segtree_add(st->left, i, j, v);
        return;
    }
    if (!st->right) {
        st->right = segtree_new(mid, st->j, 0);
    }
    if (i >= mid) {
        segtree_add(st->right, i, j, v);
        return;
    }
    // i < mid && j > mid
    segtree_add(st->left, i, mid, v);
    segtree_add(st->right, mid, j, v);
}

// Get the value of element i in the segment tree st
long segtree_get(struct segtree *st, int i) {
    if (!st) {
        fprintf(stderr, "Trying to get value of null segtree\n");
        return 0;
    }
    if (i < st->i || i >= st->j) {
        fprintf(stderr, "Trying to get value out of segtree bounds\n");
        return 0;
    }

    if (st->i == i && st->j == i) {
        return st->v;
    }

    int mid = (st->i + st->j) / 2;

    if (i < mid && st->left) {
        return st->v + segtree_get(st->left, i);
    }
    if (i >= mid && st->right) {
        return st->v + segtree_get(st->right, i);
    }

    return st->v; // All descendant values are zero 
}

