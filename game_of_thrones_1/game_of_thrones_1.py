#!/usr/bin/python3

# https://www.hackerrank.com/challenges/game-of-thrones

import sys

def is_palindrome_anagram(str):
    chars = [ False ] * 26
    for c in str:
        chars[ord(c) - 97] ^= True
    return sum(chars) <= 1

def main():
    if is_palindrome_anagram(sys.stdin.readline().strip()):
        print('YES')
    else:
        print('NO')

if __name__ == '__main__':
    main()