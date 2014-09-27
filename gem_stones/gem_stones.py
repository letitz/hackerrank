#!/usr/bin/python3

# https://www.hackerrank.com/challenges/halloween-party

import sys

def count_gems(str, gems):
	tmp = [ False ] * 26
	for c in str:
		tmp[ord(c) - 97] = True
	for i in range(26):
		if not tmp[i]:
			gems[i] = False

def main():
	N = int(sys.stdin.readline())
	gems = [ True ] * 26
	for _ in range(N):
		str = sys.stdin.readline().strip()
		count_gems(str, gems)
	print(sum(gems))

if __name__ == '__main__':
	main()