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
/*
 * Simple integer interval tree implementation
 * Kept sorted by interval starting point
 * Not balanced
 * Author: Titouan Rigoudy
 */

#include <stdlib.h>
#include <stdio.h>

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
// https://www.hackerrank.com/challenges/arithmetic-progressions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#define INTS_START_SIZE 8
#define INTS_DELIM " \t\n"
#define BUF_SIZE 10000000
#define LINE_SIZE 16
#define BIG_MOD 1000003

/* Arithmetic modulo BIG_MOD */

long INVERSES[BIG_MOD];

void calc_inverses() {
    long tmp[BIG_MOD - 2];
    long acc = 2; // powers of 2 will run the full range of the set
    int i;
    for (i = 0; i < BIG_MOD - 2; i++) {
        tmp[i] = acc;
        acc = (acc * 2) % BIG_MOD;
    }
    INVERSES[0] = 0;
    INVERSES[1] = 1;
    for (i = 0; i < BIG_MOD - 2; i++) {
        INVERSES[tmp[i]] = tmp[BIG_MOD - 3 - i];
    }
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

/* The actual logic */

struct adp {
    long a;
    long d;
    long p;
    long psum; // Cumulative sum of p values
    long dpowp; // d ^ p
    long dprod; // Cumulative product of d values
    long dpowprod; // Cumulative product of d ^ p values
};

/* Read space-separated integers */
size_t read_ints(char *line, int **ints) {
    if (line == NULL) {
        return 0;
    }
    size_t size = INTS_START_SIZE;
    *ints = malloc(INTS_START_SIZE * sizeof *ints);
    if (*ints == NULL) {
        return 0;
    }

    char *tok = line;
    char *numend;
    size_t i = 0;
    long tmp;
    while (*tok != '\n' && *tok != '\0') {
        if (isspace(*tok)) {
            tok++;
        } else if (isdigit(*tok)) {
            tmp = strtol(tok, &numend, 10);
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
            tok = numend;
        } else {
            free(*ints);
            *ints = NULL;
            return 0;
        }
    }
    return i;
}

int read_adp(char *buf, int n, struct adp *adp) {
    int *ints;
    size_t ints_len;

    int i;
    char *line;
    long dpowp;
    long psum = 0;
    long dprod = 1;
    long dpowprod = 1;
    for (i = 0; i < n; i++) {
        line = strtok(NULL, "\n");
        ints_len = read_ints(line, &ints);
        if (ints_len != 3) {
            return 1;
        }
        adp[i].a = ints[0];
        adp[i].d = ints[1];
        adp[i].p = ints[2];

        psum += adp[i].p;
        adp[i].psum = psum;

        dpowp = pow_mod(adp[i].d, adp[i].p, BIG_MOD, 1);
        adp[i].dpowp = dpowp;
        
        dprod = (dprod * adp[i].d) % BIG_MOD;
        adp[i].dprod = dprod;
        
        dpowprod = (dpowprod * dpowp) % BIG_MOD;
        adp[i].dpowprod = dpowprod;
        
        free(ints);
    }
    return 0;
}

// Calculate sum of p and product of d^p for [i, j[
void itree_mcd(
        struct adp *adp,
        struct itree *ptree,
        int i,
        int j,
        long *k,
        long *dprod)
{
    if (!ptree) {
        return;
    }
    
    bool intersects = (i < j) && (i < ptree->j) && (ptree->i < j);

    if (intersects && ptree->v > 0) {
        // We need to calculate the sum of the p values
        // and the product of the d^p at the same time
        int start = i > ptree->i ? i : ptree->i;
        int end =   j < ptree->j ? j : ptree->j;
        
        *k += ptree->v * (end-start);

        // calculate the product of all d values in [start, end[
        long dtmp = adp[end-1].dprod;
        if (i > 0) {
            dtmp = (dtmp * INVERSES[adp[start-1].dprod]) % BIG_MOD;
        }

        *dprod = pow_mod(dtmp, ptree->v, BIG_MOD, *dprod);
    }

    if (j > ptree->i) {
        // [i,j[ may intersect with children intervals to the right
        itree_mcd(adp, ptree->right, i, j, k, dprod);
    }
    
    // [i,j[ can always intersect with children intervals to the left
    itree_mcd(adp, ptree->left, i, j, k, dprod);
}

// Calculate and print for [i-1, j-1]
int min_const_diff(
    int n,
    struct adp *adp,
    struct itree *ptree,
    int i,
    int j)
{
    if (i < 1 || j > n) {
        return 1;
    }
    // k is initialized to the sum of initial p values in [i-1, j-1]
    long k = adp[j-1].psum;
    // dprod is initializaed to the product of d^p values in [i-1, j-1]
    long dprod = adp[j-1].dpowprod;
    if (i >= 2) {
        k -= adp[i-2].psum;
        dprod = (dprod * INVERSES[adp[i-2].dpowprod]) % BIG_MOD;
    }
    itree_mcd(adp, ptree, i-1, j, &k, &dprod);
    long v = factorial_mod(k, BIG_MOD, dprod);
    printf("%ld %ld\n", k, v);
    return 0;
}

void update_adp(struct adp *adp, struct ievent *events, int n) {
    if (!events || n <= 0) {
        return;
    }
    int i = 0;
    int p = 0;
    int psum = 0;
    int dpowprod = 1;
    while (true) {
        // Compress all events at the same index into one
        int start = events[i].i;
        while (i < n && events[i].i == start) {
            p += events[i].v;
            i++;
        }

        if (i == n) {
            // The last event should close the last remaining interval
            assert(p == 0);
            break;
        }
        
        int end = events[i].i; // next event

        // p is now constant on [start, end[
        for (int j = start; j < end; j++) {
            adp[j].p += p;

            psum += p;
            adp[j].psum += psum;

            adp[j].dpowp = pow_mod(adp[j].d, adp[j].p, BIG_MOD, 1);

            dpowprod = (dpowprod * adp[j].dpowp) % BIG_MOD;
            adp[j].dpowprod = dpowprod;
        }
    }
}

int ptree_size = 0;

int add_powers(int n, struct adp *adp, struct itree *ptree, int i, int j, int v) {
    if (i < 1 || j > n) {
        return 1;
    }
    itree_add(ptree, i-1, j, v);
    ptree_size++;
    if (ptree_size * ptree_size >= n) {
        struct ievent *events = NULL;
        int n_events = itree_flatten(ptree, &events);
        update_adp(adp, events, n_events);
        free(events);
        ptree_size = 0;
    }
    return 0;
}

int handle_query(char *str, int n, struct adp *adp, struct itree *ptree) {
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
    // Prep work
    calc_inverses();

    char *buf = malloc(BUF_SIZE * sizeof *buf);
    if (buf == NULL) {
        return 0;
    }
    fread(buf, BUF_SIZE, 1, stdin);

    char *line = strtok(buf, "\n");

    char *end;
    int n = strtol(line, &end, 10);
    if ((!isspace(*end) && *end != '\0') || n < 1) {
        return 0;
    }

    struct adp *adp = malloc(n * sizeof *adp);
    struct itree *ptree = itree_new(0, n, 0);
    if (adp == NULL || ptree == NULL) {
        return 0;
    }
    int err = read_adp(buf, n, adp);
    if (err) {
        return 0;
    }

    line = strtok(NULL, "\n");
    int q = strtol(line, &end, 10);
    if ((!isspace(*end) && *end != '\0') || q < 1) {
        return 0;
    }

    int i;
    for (i = 0; i < q; i++) {
        line = strtok(NULL, "\n");
        err = handle_query(line, n, adp, ptree);
        if (err) {
            return 0;
        }
    }

    return 0;
}
