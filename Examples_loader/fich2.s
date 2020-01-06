	.cpu arm7tdmi
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"fich2.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hello\000"
	.text
	.align	2
	.global	salut
	.syntax unified
	.arm
	.fpu softvfp
	.type	salut, %function
salut:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	ldr	r0, .L2
	bl	puts
	nop
	pop	{fp, pc}
.L3:
	.align	2
.L2:
	.word	.LC0
	.size	salut, .-salut
	.ident	"GCC: (15:6.3.1+svn253039-1build1) 6.3.1 20170620"
