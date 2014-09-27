#!/usr/bin/python3

import sys

def nextints():
	return [int(x) for x in sys.stdin.readline().split()]

def maxwidth_aux(width, i, j):
	res = 3
	for w in width[i:j+1]:
		if w == 1:
			return 1
		res = min(res,w)
	return res

def maxwidth(width, i, j, memo):
	res = memo.get((i,j), None)
	if res:
		return res
	res = maxwidth_aux(width, i, j)
	memo[(i,j)] = res
	return res

def main():
	[N,T] = nextints()
	width = nextints()
	memo = dict()
	for _ in range(T):
		[i,j] = nextints()
		print(maxwidth(width, i, j, memo))

main()
	