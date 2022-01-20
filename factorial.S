    .intel_syntax noprefix
    .global regulafalsi

    .text

    .align 16

factorial:
	cmp rdi , 34
    ja 3f
	xor edx, edx // Result stored in rdx:rax mov eax, 1
    mov eax, 1
	cmp edi, 1
	jbe 2f
1:
	mov rcx, rdi
	imul rcx, rdx
	mul rdi
	sub edi, 1
	add rdx, rcx
	cmp edi, 1
	ja 1b
2:  ret
3:	mov rax, -1
    mov rdx , -1
    ret