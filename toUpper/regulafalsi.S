    .intel_syntax noprefix
    .global regulafalsi

    .text

    .align 16

regulafalsi:

movdqu xmm4, .LSB[rip]
movdqu xmm2, .LSZ[rip]
movdqu xmm3, .LTZ[rip]

sub rsp, 0x18 // save space for 1 whole xmm register(128bit), and 8 bytes to align stack
check: //checks if pointer is 16 byte aligned
  mov eax, 0xF
  and eax, edi
  cmp al, 0
  je vector
start: // loads 1 char at a time
  mov al, [rdi]
  cmp al, 0
  je .Lend
  sub al,'a'                  
  cmp al,25                 
  ja .Lcont                 
  sub byte ptr [rdi],0x20 
.Lcont:
  inc rdi
  jmp check

vector:
pxor xmm1, xmm1
movdqa xmm0, xmmword ptr [rdi]
pcmpeqb xmm1, xmm0
movdqa xmmword ptr[rsp], xmm1

movq rax, [rsp]
add rax, QWORD PTR [rsp+8]
cmp rax, 0
jne start

movdqu xmm1, xmm4 // mov [0x7B,...,0x7B] in tmp register xmm1
pcmpgtb xmm1, xmm0 // 0xFF in all bytes xmm5[index], where string[index] < 0x7B 
pand xmm1, xmm0 // IF (str[i] < 0x7B) THEN xmm1[i] = str[i] ELSE 0
pcmpgtb xmm1, xmm2 // IF (str[i] > 0x60 && str[i] < 0x7B) THEN xmm1[i] = 0xFF ELSE 0
pand xmm1, xmm3 // IF (str[i] > 0x60 && str[i] < 0x7B) THEN xmm1[i] = 0x20 ELSE 0

psubb xmm0, xmm1
movdqa xmmword ptr [rdi], xmm0

// next loop
add rdi, 16
jmp vector

.Lend:
  add rsp, 0x18 // clean stack
  ret
 
.LSZ:
	.long 0x60606060
    .long 0x60606060
.LSB:
	.long 0x7b7b7b7b
    .long 0x7b7b7b7b
.LTZ:
	.long 0x20202020
    .long 0x20202020