#!/usr/bin/python3

import sys

def calcsize(cycles):
	tmp = 2**((cycles+3)//2)
	if cycles % 2 == 1:
		return tmp - 2
	return tmp - 1

def main():
	total = int(sys.stdin.readline())
	for i in range(total):
		print(calcsize(int(sys.stdin.readline())))

main()