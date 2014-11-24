#ifndef SEGTREE_H
#define SEGTREE_H

/*
 * Simple integer segment tree implementation
 * Author: Titouan Rigoudy
 */

struct segtree {
    int i;
    int j;
    long v;
    struct segtree *left;
    struct segtree *right;
};

struct segtree *segtree_new(int i, int j, long v);

struct segtree *segtree_new_simple(int n);

void segtree_free(struct segtree *st);

void segtree_print(struct segtree *st);

void segtree_add(struct segtree *st, int i, int j, long v);

long segtree_get(struct segtree *st, int i);

#endif // SEGTREE_H
