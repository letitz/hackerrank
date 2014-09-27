#!/usr/bin/python3

# https://www.hackerrank.com/challenges/filling-jars

import sys

def nextints():
    return [ int(x) for x in sys.stdin.readline().split() ]

def new_candies(start, end, new_per_jar):
    return (end - start + 1) * new_per_jar

def main():
    total = 0
    [num_jars, num_ops] = nextints()
    for _ in range(num_ops):
        [start, end, new_per_jar] = nextints()
        total += new_candies(start, end, new_per_jar)
    print(total // num_jars)

if __name__ == '__main__':
    main()