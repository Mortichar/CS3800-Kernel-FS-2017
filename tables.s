	.file	"tables.c"
	.data
	.align 32
	.type	GDT_entries, @object
	.size	GDT_entries, 72
GDT_entries:
	.quad	0
	.quad	0
	.byte	0
	.zero	7
	.quad	0
	.quad	4294967295
	.byte	-102
	.zero	7
	.quad	0
	.quad	4294967295
	.byte	-110
	.zero	7
	.local	GDT
	.comm	GDT,24,16
	.text
	.globl	encodeGdtEntry
	.type	encodeGdtEntry, @function
encodeGdtEntry:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	24(%rbp), %rax
	cmpq	$65536, %rax
	movq	24(%rbp), %rax
	cmpq	$65536, %rax
	jbe	.L3
	movq	24(%rbp), %rax
	shrq	$12, %rax
	movq	%rax, 24(%rbp)
	movq	-8(%rbp), %rax
	addq	$6, %rax
	movb	$-64, (%rax)
	jmp	.L4
.L3:
	movq	-8(%rbp), %rax
	addq	$6, %rax
	movb	$64, (%rax)
.L4:
	movq	24(%rbp), %rax
	movl	%eax, %edx
	movq	-8(%rbp), %rax
	movb	%dl, (%rax)
	movq	24(%rbp), %rax
	shrq	$8, %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	$1, %rax
	movb	%dl, (%rax)
	movq	-8(%rbp), %rax
	addq	$6, %rax
	movzbl	(%rax), %edx
	movq	24(%rbp), %rax
	shrq	$16, %rax
	andl	$15, %eax
	movl	%eax, %ecx
	movq	-8(%rbp), %rax
	addq	$6, %rax
	orl	%ecx, %edx
	movb	%dl, (%rax)
	movq	16(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	$2, %rax
	movb	%dl, (%rax)
	movq	16(%rbp), %rax
	shrq	$8, %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	$3, %rax
	movb	%dl, (%rax)
	movq	16(%rbp), %rax
	shrq	$16, %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	$4, %rax
	movb	%dl, (%rax)
	movq	16(%rbp), %rax
	shrq	$24, %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	$7, %rax
	movb	%dl, (%rax)
	movq	-8(%rbp), %rax
	leaq	5(%rax), %rdx
	movzbl	32(%rbp), %eax
	movb	%al, (%rdx)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	encodeGdtEntry, .-encodeGdtEntry
	.globl	create_gdt
	.type	create_gdt, @function
create_gdt:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, %eax
	movb	%al, -36(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movb	$0, -9(%rbp)
	jmp	.L6
.L7:
	movzbl	-9(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movq	-8(%rbp), %rdx
	pushq	16(%rax)
	pushq	8(%rax)
	pushq	(%rax)
	movq	%rdx, %rdi
	call	encodeGdtEntry
	addq	$24, %rsp
	addq	$8, -8(%rbp)
	movzbl	-9(%rbp), %eax
	addl	$1, %eax
	movb	%al, -9(%rbp)
.L6:
	movzbl	-9(%rbp), %eax
	cmpb	-36(%rbp), %al
	jb	.L7
	movq	-8(%rbp), %rax
	movl	$8, %esi
	movq	%rax, %rdi
	call	set_gdt
	movl	$0, %eax
	call	reload_segments
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	create_gdt, .-create_gdt
	.globl	set_gdt
	.type	set_gdt, @function
set_gdt:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	leaq	-32(%rbp), %rsi
#APP
# 49 "tables.c" 1
	movl %rdx, 0x2(%rsi) ;
movw %rcx, (%rsi) ;
lgdt  (%rsi)
# 0 "" 2
#NO_APP
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L9
	call	__stack_chk_fail@PLT
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	set_gdt, .-set_gdt
	.globl	reload_segments
	.type	reload_segments, @function
reload_segments:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
#APP
# 61 "tables.c" 1
	   ljmp   $0x08, $reload_CS
reload_CS:
   movw   $0x10, %ax
   movw   %ax,  %ds
   movw   %ax,  %es
   movw   %ax,  %fs
   movw   %ax,  %gs
   movw   %ax,  %ss

# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	reload_segments, .-reload_segments
	.ident	"GCC: (GNU) 7.2.0"
	.section	.note.GNU-stack,"",@progbits
