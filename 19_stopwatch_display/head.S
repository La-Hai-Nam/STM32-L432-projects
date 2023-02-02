	.global _start

	.equ STACK_HIGH, 0x20010000 /* Use 0x20001800 on an F0xxx */

Vectors:
	.word STACK_HIGH /* initial value of the sp */
	.word _start + 1

	.org 0x003C
	.word systick + 1