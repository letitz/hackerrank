#!/usr/bin/python3

# https://www.hackerrank.com/challenges/arithmetic-progressions

import sys, math

def read_ints():
    return [int(x) for x in sys.stdin.readline().split()]

def read_sequences(n):
    a = [0] * n
    d = [0] * n
    p = [0] * n
    for i in range(n):
        ints = read_ints()
        a[i] = ints[0]
        d[i] = ints[1]
        p[i] = ints[2]
    return (a, d, p)

def pow_mod(a, b, mod, acc):
    powa = a % mod
    while b > 0: 
        if b & 1 == 1:
            acc *= powa % mod
        powa *= powa % mod
        b >>= 1
    return acc

def factorial_mod(n, mod, acc):
    for i in range(2, n+1):
        acc = (acc * i) % mod
    return acc

def min_const_diff(a, d, p, i, j):
    k = 0
    v = 1
    for l in range(i-1, j):
        if p[l] > 0:
            k += p[l]
            if d[l] == 0:
                v = pow_mod(a[l], p[l], 1000003, v)
            else:
                v = pow_mod(d[l], p[l], 1000003, v)
    v = factorial_mod(k, 1000003, v)
    return (k, v)

def add_powers(p, i, j, v):
    for k in range(i-1, j):
        p[k] += v

def answer_query(a, d, p):
    ints = read_ints()
    if ints[0] == 0:
        k, v = min_const_diff(a, d, p, ints[1], ints[2])
        print('{:d} {:d}'.format(k, v))
    elif ints[0] == 1:
        add_powers(p, ints[1], ints[2], ints[3])

def main():
    n = int(sys.stdin.readline())
    a, d, p = read_sequences(n)
    q = int(sys.stdin.readline())
    for i in range(q):
        answer_query(a, d, p)

if __name__ == '__main__':
    main()