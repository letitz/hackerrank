#ifndef ITREE_H
#define ITREE_H

/*
 * Simple integer interval tree implementation
 * Author: Titouan Rigoudy
 */

struct itree {
    int i;
    int j;
    long v;
    struct itree *left;
    struct itree *right;
};

// Describes the start or end of an interval
struct ievent {
    int i;
    long v; // i.v for interval start, -i.v for interval end
};

struct itree *itree_new(int i, int j, long v);

void itree_free(struct itree *it);

void itree_print(struct itree *it);

void itree_add(struct itree *it, int i, int j, long v);

// Flatten the tree into a list of events representing
// interval start and end points
int itree_flatten(struct itree *it, struct ievent **events);

#endif
