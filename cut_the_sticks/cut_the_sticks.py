#!/usr/bin/python3

# https://www.hackerrank.com/challenges/cut-the-sticks

import sys

def print_cuts(N, lengths):
    max_length = 0
    sticks = [ 0 ] * 1000
    for length in lengths:
        sticks[length] += 1
        max_length = max(max_length,length)

    for length in range(max_length + 1):
        if sticks[length] > 0:
            print(N)
            N -= sticks[length]

def main():
    N = int(sys.stdin.readline())
    lengths = [ int(x) for x in sys.stdin.readline().split() ]
    print_cuts(N,lengths)

if __name__ == '__main__':
    main()