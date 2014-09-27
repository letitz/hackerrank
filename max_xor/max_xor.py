#!/usr/bin/python3

# https://www.hackerrank.com/challenges/maximizing-xor

import sys

def max_xor(L,R):
    res = 1
    n = L ^ R
    while n > 0:
        n >>=1
        res <<= 1
    return res - 1

def main():
    L = int(sys.stdin.readline())
    R = int(sys.stdin.readline())
    print(max_xor(L,R))

if __name__ == '__main__':
    main()
