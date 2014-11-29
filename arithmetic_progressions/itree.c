/*
 * Simple integer interval tree implementation
 * Kept sorted by interval starting point
 * Not balanced
 * Author: Titouan Rigoudy
 */

#include <stdlib.h>
#include <stdio.h>

#include "itree.h"

#define EVENTS_MIN_SIZE 1024

struct itree *itree_new(int i, int j, long v) {
    struct itree *it = malloc(sizeof *it);
    if (!it) {
        return NULL;
    }
    it->i = i;
    it->j = j;
    it->v = v;
    it->left = NULL;
    it->right = NULL;
    return it;
}

void itree_free(struct itree *it) {
    if (!it) {
        return;
    }
    itree_free(it->left);
    itree_free(it->right);
    free(it);
}

void itree_print(struct itree *it) {
    if (!it) {
        puts("NULL itree");
    }
    printf("itree{i: %d, j:%d, v:%ld}\n", it->i, it->j, it->v);
    puts("Left:");
    if (it->left) {
        itree_print(it->left);
    }
    puts("Right:");
    if (it->right) {
        itree_print(it->right);
    }
}

void itree_add(struct itree *it, int i, int j, long v) {
    if (!it) {
        fputs("Adding to null interval tree", stderr);
        return;
    }

    if (it->i == i && it->j == j) {
        it->v += v;
        return;
    }
    
    if (i < it->i || (i == it->i && j <= it->j)) {
        if (it->left) {
            itree_add(it->left, i, j, v);
        } else {
            it->left = itree_new(i, j, v);
        }
        return;
    }
    
    if (it->right) {
        itree_add(it->right, i, j, v);
    } else {
        it->right = itree_new(i, j, v);
    }
}

// For use in qsort
int ieventcmp(const void *event1, const void *event2) {
    const struct ievent *ev1 = event1;
    const struct ievent *ev2 = event2;
    return ev1->i - ev2->i;
}

// For debugging purposes
void ievent_print(const struct ievent ev) {
    printf("ievent{i:%d, v:%ld}\n", ev.i, ev.v);
}

// Recursively add events from it to *events
// *events is of size *n, *i is the current number of events in *events
// Returns 0 on success, 1 on error
int itree_flatten_aux(
        struct itree *it, struct ievent **events, int *n, int *i) {

    if (!it) {
        return 0;
    }
    
    int err = itree_flatten_aux(it->left, events, n, i);
    if (err) {
        return err;
    }
    free(it->left);
    it->left = NULL;
    
    if (*i == *n) {
        // grow events
        *n = *n * 2 + 2; // just in case n was 0
        *events = realloc(*events, *n);
        if (*events == NULL) {
            return 1; // signal error
        }
    }
    if (it->v != 0) {
        // Add interval start event
        (*events)[*i].i = it->i;
        (*events)[*i].v = it->v;
        (*i)++;
        // Add interval end event
        (*events)[*i].i = it->j;
        (*events)[*i].v = -it->v;
        (*i)++;
    }
 
    err = itree_flatten_aux(it->right, events, n, i);
    if (err) {
        return err;
    }
    free(it->right);
    it->right = NULL;

    it->v = 0;

    return 0;
}

// Flatten the tree into a list of events representing
// interval start and end points
int itree_flatten(struct itree *it, struct ievent **events) {
    int n = EVENTS_MIN_SIZE;
    *events = malloc(n * sizeof(**events));
    if (*events == NULL) {
        return 0;
    }
    int i = 0;
    int err = itree_flatten_aux(it, events, &n, &i);
    if (err) {
        *events = NULL;
        return 0;
    }
    qsort(*events, i, sizeof(**events), ieventcmp);
    return i;     
}
