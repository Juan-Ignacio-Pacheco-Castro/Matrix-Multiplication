
# %r10 y %r11 son calleer saved

.section .text                  # inicia la seccion de codigo
    .globl quickMatrixMul       # declara quickMatrixMul como nombre global
quickMatrixMul:
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
        leaq (,%rax,4), %r10		# %r10 = (i*m + k) * sizeof(float)
        movq %rdi, %r11			# %r11 = matrixNM
        addq %r10, %r11			# %r11 += (i*m + k) * sizeof(float)
        pxor %xmm0, %xmm0
        movss (%r11), %xmm0		# %xmm0 = r = matrixNM[i*m + k]
        shufps $0, %xmm0, %xmm0         # %xmm0 = |r|r|r|r|
        movl $0, 0xC(%rsp)		# j = 0
L3:					# Tercer for (j)
        cmpl %r9d, 0xC(%rsp)		# j - p
        jnb L4                          # j < p ? Si no, brinque

        movl %r9d, %r10d                 # r10d = p
        cmpl $4,%r10d
        jb L7
        subl 0xC(%rsp), %r10d            # r10d = p - j
        testl $0xFFFFFFFC,%r10d
        je L6
        #cmpl $4, %r10d
        #jge L6                        # p - j < 4 ? L6 : L7     Jump if not above
L7:                                     # Seccion secuencial de SSE. Se usa suffix ss
        cmpl %r9d, 0xC(%rsp)		# j - p
        jnb L4                          # j < p ? Si no, brinque
        movl 0x10(%rsp), %eax		# %eax = k
        imull %r9d, %eax		# %eax == k *= p
        movl 0xC(%rsp), %r10d		# %r10 = j
        addl %r10d, %eax		# %eax == k*p + j
        leaq (,%rax,4), %r10		# %rax = %rax * sizeof(float)
        movq %rsi, %r11			# %r11 = matrixMP
        addq %r10, %r11			# %r11 += (k*p + j) * sizeof(float)
        pxor %xmm2, %xmm2
        movss (%r11), %xmm2		# %xmm2 = matrixMP[k*p + j]
        mulss %xmm0, %xmm2		# %xmm2 = r * matrixMP[k*p + j]...

# i*p + j
# matrixNP[i*p + j]
        movl 0x8(%rsp), %eax		# %rax = i
        imull %r9d, %eax		# %rax == i *= p
        movl 0xC(%rsp), %r10d		# %r10 = j
        addl %r10d, %eax		# %eax == i*p + j
        leaq (,%rax,4), %rax		# %rax = %rax * sizeof(float)
        movq %rdx, %r11			# %r11 = matrixNP
        addq %rax, %r11			# %r11 += (i*p + j) * sizeof(float)

        pxor %xmm1, %xmm1
        movss (%r11), %xmm1		# %xmm1 = matrixNP[i*p + j], accede al float
        addss %xmm1, %xmm2		# %xmm2 = matrixNP[i*p + j] + r * matrixMP[k*p + j];
        movss %xmm2, (%r11)		# matrixNP[i*p + j] = %xmm2
        addl $1, 0xC(%rsp)		# j += 1
        jmp L7				# termina iteracion j de 1


L6:                                     # Seccion paralela de SSE. Se usa suffix ps
        movl 0x10(%rsp), %eax		# %eax = k
        imull %r9d, %eax		# %eax == k *= p
        movl 0xC(%rsp), %r10d		# %r10 = j
        addl %r10d, %eax		# %eax == k*p + j
        leaq (,%rax,4), %r10		# %rax = %rax * sizeof(float)
        movq %rsi, %r11			# %r11 = matrixMP
        addq %r10, %r11			# %r11 += (k*p + j) * sizeof(float)
        pxor %xmm2, %xmm2
        movaps (%r11), %xmm2		# %xmm2 = |matrixMP[k*p + j]|=+1|=+2|=+3|
        mulps %xmm0, %xmm2		# %xmm2 = 4r * 4matrixMP[k*p + j]...
	
# i*p + j
# matrixNP[i*p + j]
        movl 0x8(%rsp), %eax		# %rax = i
        imull %r9d, %eax		# %rax == i *= p
        movl 0xC(%rsp), %r10d		# %r10 = j
        addl %r10d, %eax		# %eax == i*p + j
	leaq (,%rax,4), %rax		# %rax = %rax * sizeof(float)
        movq %rdx, %r11			# %r11 = matrixNP
        addq %rax, %r11			# %r11 += (i*p + j) * sizeof(float)

        pxor %xmm1, %xmm1
        movaps (%r11), %xmm1		# %xmm1 = matrixNP[i*p + j], accede al float
        addps %xmm1, %xmm2		# %xmm2 = matrixNP[i*p + j] + r * matrixMP[k*p + j];
        movaps %xmm2, (%r11)		# matrixNP[i*p + j] = %xmm2
        addl $4, 0xC(%rsp)		# j += 4
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
