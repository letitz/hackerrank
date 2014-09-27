#!/usr/bin/python3

# https://www.hackerrank.com/challenges/chocolate-feast

import sys

def num_chocolates(money, cost, exchange):
    choco = money // cost
    wrappers = choco
    while wrappers >= exchange:
        exch_choco, wrappers = divmod(wrappers, exchange)
        wrappers += exch_choco
        choco += exch_choco
    return choco

def main():
    T = int(sys.stdin.readline())
    for _ in range(T):
        [N, C, M] = [ int(x) for x in sys.stdin.readline().split() ]
        print(num_chocolates(N, C, M))

if __name__ == '__main__':
    main()