#!/usr/bin/python3

# https://www.hackerrank.com/challenges/sherlock-and-gcd

import sys
from fractions import gcd

def gcd_all(numbers):
    res = 0
    for n in numbers:
        res = gcd(res, n)
    return res

def main():
    T = int(sys.stdin.readline())
    for _ in range(T):
        N = int(sys.stdin.readline())
        A = [ int(x) for x in sys.stdin.readline().split() ]
        if gcd_all(A) == 1:
            print('YES')
        else:
            print('NO')

if __name__ == '__main__':
    main()