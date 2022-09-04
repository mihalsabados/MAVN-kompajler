.globl main

.data
m1:	.word 6
m2:	.word 5

.text
main:
	la	$t1, m1
	la	$t0, m2
	or	$t1, $t1, $t0
	mult	$t0, $t1
	seq	$t0, $t1, $t0
