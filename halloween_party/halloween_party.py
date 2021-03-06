#!/usr/bin/python3

# https://www.hackerrank.com/challenges/halloween-party

import sys

def max_pieces(n):
    return (n // 2) * ((n+1) // 2)

def main():
    T = int(sys.stdin.readline())
    for _ in range(T):
        print(max_pieces(int(sys.stdin.readline())))

if __name__ == '__main__':
    main()