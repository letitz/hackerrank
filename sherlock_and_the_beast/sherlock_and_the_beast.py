#!/usr/bin/python3

# https://www.hackerrank.com/challenges/sherlock-and-the-beast

import sys

def largest_decent_number(num_digits):
    if num_digits in [1,2,4,7]:
        return '-1'
    quot3, mod3 = divmod(num_digits, 3)
    if mod3 == 0:
        return '555' * quot3
    elif mod3 == 1:
        return '555' * (quot3 - 3) + '3333333333'
    elif mod3 == 2:
        return '555' * (quot3 - 1) + '33333'


def main():
    num_tests = int(sys.stdin.readline())
    for _ in range(num_tests):
        print(largest_decent_number(int(sys.stdin.readline())))

if __name__ == '__main__':
    main()