#!/usr/bin/python3

# https://www.hackerrank.com/challenges/the-love-letter-mystery

import sys

def char_distance(ca,cb):
	return abs(ord(ca) - ord(cb))

def palindrome_distance(str):
	length = len(str)
	dist = 0
	for i in range(length // 2):
		dist += char_distance(str[i], str[-i-1])
	return dist

def main():
	T = int(sys.stdin.readline())
	for _ in range(T):
		print(palindrome_distance(sys.stdin.readline().strip()))

main()