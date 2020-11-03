
# %r10 y %r11 son calleer saved

.section .text                  # inicia la seccion de codigo
    .globl quickMatrixMul       # declara quickMatrixMul como nombre global
quickMatrixMul:
/*
        subq $32, %rsp       		# Restele 32 bytes al rsp, espacio para i, j, k
                                        # 0x8(%rsp) = i
                                        # 0xC(%rsp) = j
                                        # 0x10(%rsp) = k
        movl $0, 0x10(%rsp)		# k = 0

L1:					# Primer for, ciclo externo (k)
        cmpl %r8d, 0x10(%rsp)		# k - m
        jnb end			# k < m ? Si no, brinque
        movl $0, 0x8(%rsp)		# i = 0

L2:					# Segundo for (i)
        cmpl %ecx, 0x8(%rsp)		# i - n
        jnb L5			# i < n ? Si no, brinque
        movl 0x8(%rsp), %eax		# %eax = i
        imull %r8d, %eax		# %eax == i *= m
        movl 0x10(%rsp), %r10d		# %r10 = k
        addl %r10d, %eax		# %eax == i*m + k
        # movl %eax, %eax
        leaq (,%rax,4), %r10		# %r10 = (i*m + k) * sizeof(float)
        movq %rdi, %r11			# %r11 = matrixNM
        addq %r10, %r11			# %r11 += (i*m + k) * sizeof(float)
        pxor %xmm0, %xmm0
        movss (%r11), %xmm0		# %xmm0 = r = matrixNM[i*m + k]
        movl $0, 0xC(%rsp)		# j = 0

L3:					# Tercer for (j)
        cmpl %r9d, 0xC(%rsp)		# j - p
        jnb L4	                # j < p ? Si no, brinque

        movl 0x10(%rsp), %eax		# %eax = k
        imull %r9d, %eax		# %eax == k *= p
        movl 0xC(%rsp), %r10d		# %r10 = j
        addl %r10d, %eax		# %eax == k*p + j
        leaq (,%rax,4), %r10		# %rax = %rax * sizeof(float)
        movq %rsi, %r11			# %r11 = matrixMP
        addq %r10, %r11			# %r11 += (k*p + j) * sizeof(float)
        pxor %xmm2, %xmm2
        movss (%r11), %xmm2		# %xmm2 = matrixMP[k*p + j], accede al float
        mulss %xmm0, %xmm2		# %xmm2 = r * matrixMP[k*p + j]
	
        movl 0x8(%rsp), %eax		# %rax = i
        imull %r9d, %eax		# %rax == i *= p
        movl 0xC(%rsp), %r10d		# %r10 = j
        addl %r10d, %eax		# %eax == i*p + j
        # movl %eax, %eax
	leaq (,%rax,4), %rax		# %rax = %rax * sizeof(float)
        movq %rdx, %r11			# %r11 = matrixNP
        addq %rax, %r11			# %r11 += (i*p + j) * sizeof(float)

        pxor %xmm1, %xmm1
        movss (%r11), %xmm1		# %xmm1 = matrixNP[i*p + j], accede al float

        addss %xmm1, %xmm2		# %xmm2 = matrixNP[i*p + j] + r * matrixMP[k*p + j];
        movss %xmm2, (%r11)		# matrixNP[i*p + j] = %xmm2
        addl $1, 0xC(%rsp)		# ++j
        jmp L3				# termina iteracion j
L4:
        addl $1, 0x8(%rsp)		# ++i
        jmp L2				# Termina ciclo de j, salte a ciclo de i
L5:
        addl $1, 0x10(%rsp)		# ++k
        jmp L1                          # Vuelve al ciclo externo
end:
        addq $32, %rsp                  # Restura la pila
	ret
*/

movaps (%rsi), %xmm1        # xmm1 = |4.0|2.0|2.0|3.0|
#    mulps %xmm1, %xmm0
#   movlhps %xmm2, %xmm2

movss (%rdi), %xmm0         # xmm0 = |0|0|0|1.00|
shufps $0, %xmm0, %xmm0     # xmm0 = |1.00|1.00|1.00|1.00|
mulps %xmm1, %xmm0          #      = |1.00  * 4.00|
movaps %xmm0, (%rdx)
# movaps %xmm1, (%rdx)

/*
# --------------------------

leaq 0x4(%rsi), %rax
movaps (%rax), %xmm1
movss (%rdi), %xmm0
shufps $0, %xmm0, %xmm0
mulps %xmm1, %xmm0

leaq 0x4(%rdx), %rax
movaps %xmm0, (%rax)
*/




