    .intel_syntax noprefix
    .global numquad
    .global fn_x2

    .text

    .align 16
numquad: // double numquad(double(* f)(double), double a, double b, size_t n)
    // rdi <- fn
    // xmm0 <- a
    // xmm1 <- b
    // rsi <- n
    // xmm2 <- schrittweite
    // xmm9 <- sumResult
    // xmm10 <- 2.0
    // rbx <- fn, a copy, because rdi is temporary register
    // r12 <- n, a copy

    //xmm4 <- left
    //xmm5 <- right


    push rbx
    push r12
    mov rbx, rdi // copy of fn pointer
    mov r12, rsi

    cmp r12, 2
    jl nan

    dec r12 // to compute the schrittweite
    cvtsi2sd xmm3, r12
    movsd xmm2, xmm1 // move b to tmp register
    subsd xmm2,xmm0 // tmp = b - a
    divsd xmm2,xmm3 // tmp /= (n-1)

    pxor xmm9, xmm9 // sumResult = 0
    movsd xmm5, xmm0 // init right=a

    loop:
        movsd xmm4, xmm5 // left_new = right_old

        movsd xmm5, xmm4 // right_new = left_new + schrittweite
        addsd xmm5, xmm2

        movsd xmm6, xmm5 // xmm6 = (b-a)
        subsd xmm6, xmm4

        sub rsp, 8
        sub rsp, 16*5
        movq [rsp], xmm2
        movq [rsp+16], xmm4
        movq [rsp+32], xmm5
        movq [rsp+48], xmm6
        movq [rsp+64], xmm9


        movsd xmm0, xmm5 // xmm7 = f(b)
        call rbx
        movsd xmm7, xmm0


        movq xmm9, [rsp+64]
        movq xmm6, [rsp+48]
        movq xmm5, [rsp+32]
        movq xmm4, [rsp+16]
        movq xmm2, [rsp]
        add rsp, 16*5
        add rsp, 8



        sub rsp, 8
        sub rsp, 16*5
        movq [rsp], xmm2
        movq [rsp+16], xmm4
        movq [rsp+32], xmm5
        movq [rsp+48], xmm6
        movq [rsp+64], xmm9



        movsd xmm0, xmm4 // xmm8 = f(a)
        call rbx
        movsd xmm8, xmm0


        movq xmm9, [rsp+64]
        movq xmm6, [rsp+48]
        movq xmm5, [rsp+32]
        movq xmm4, [rsp+16]
        movq xmm2, [rsp]
        add rsp, 16*5
        add rsp, 8



        addsd xmm7, xmm8 // xmm7 = f(b) + f(a)
        movsd xmm0, xmm7
        mulsd xmm6, xmm7 // xmm6 = (b-a) * (f(b) + f(a))

        addsd xmm9, xmm6 // sumResult += xmm6

        dec r12 // for n Stellen, we iterate n-1 times. We decrement rsi until 0 is reached
        cmp r12, 0
        jne loop // repeat


        mov rcx, 2
        cvtsi2sd xmm10 ,rcx // for the division in the formula: (b−a)·(f(b)+f(a))/2
        divsd xmm9, xmm10
        //movsd xmm0, xmm9

        pop r12
        pop rbx
        ret

    nan:
        pxor xmm0, xmm0
        divsd xmm0, xmm0
        pop r12
        pop rbx
        ret


    .align 16
fn_x2: // double fn_x2(double)
    // This function makes full use of its rights granted by the ABI.
    // No need to reduce the stack pointer, as the ABI defines
    // a freely usable "red zone" of 128 Bytes below rsp.

    // Check whether stack is suitably aligned.
    movaps [rsp - 0x18], xmm0
    pxor xmm0, xmm0; pxor xmm1, xmm1; pxor xmm2, xmm2; pxor xmm3, xmm3
    pxor xmm4, xmm4; pxor xmm5, xmm5; pxor xmm6, xmm6; pxor xmm7, xmm7
    pxor xmm8, xmm8; pxor xmm9, xmm9; pxor xmm10, xmm10; pxor xmm11, xmm11
    pxor xmm12, xmm12; pxor xmm13, xmm13; pxor xmm14, xmm14; pxor xmm15, xmm15
    xor eax, eax; xor ecx, ecx; xor edx, edx; xor esi, esi; xor edi, edi
    xor r8, r8; xor r9, r9; xor r10, r10; xor r11, r11

    movsd xmm0, [rsp - 0x18]
    mulsd xmm0, xmm0
    ret
