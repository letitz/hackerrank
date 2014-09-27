#!/usr/bin/python3

# https://www.hackerrank.com/challenges/manasa-and-stones

import sys

def print_possible_values(num_stones, a, b):
    if num_stones <= 1:
        print(0)
        return
    if a == b:
        print((num_stones - 1) * a)
        return
    if a > b:
        tmp = a
        a = b
        b = tmp

    mini = a * (num_stones - 1)
    maxi = b * (num_stones - 1)
    step = b - a
    values = [ str(n) for n in range(mini, maxi + 1, step) ]
    print(' '.join(values))

def main():
    num_tests = int(sys.stdin.readline())
    for _ in range(num_tests):
        num_stones = int(sys.stdin.readline())
        a = int(sys.stdin.readline())
        b = int(sys.stdin.readline())
        print_possible_values(num_stones, a, b)

if __name__ == '__main__':
    main()