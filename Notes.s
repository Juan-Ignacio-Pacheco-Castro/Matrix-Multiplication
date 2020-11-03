


%rdi = matrixNM
%rsi = matrixMP
%rdx = matrixNP
%rcx = n
%r8 = m
%r9 = p


/*

movaps (%rsi), %xmm1
#    mulps %xmm1, %xmm0
#   movlhps %xmm2, %xmm2

movss (%rdi), %xmm0
shufps $0, %xmm0, %xmm0
mulps %xmm1, %xmm0
movaps %xmm0, (%rdx)


# --------------------------

leaq 0x4(%rsi), %rax
movaps (%rax), %xmm1
movss (%rdi), %xmm0
shufps $0, %xmm0, %xmm0
mulps %xmm1, %xmm0

leaq 0x4(%rdx), %rax
movaps %xmm0, (%rax)

*/
