    .intel_syntax noprefix
    .global numquad
    .global fn_x2

    .text

    .align 16
numquad: // double numquad(double(* f)(double), double a, double b, size_t n)
    // TODO: implement this function
    //rdi <- fn
    // xmm0 <- a
    // xmm1 <- b
    // rsi <- n
    // xmm2 <- schrittweite
    // xmm9 <- sumResult
    // xmm10 <- 2.0

    //xmm4 <- left
    //xmm5 <- right


    cmp rsi, 2
    jl nan

    cvtsi2sd xmm3, rsi // to compute the schrittweite
    movsd xmm2, xmm1 // move b to tmp register
    subsd xmm2,xmm0 // tmp = b - a
    divsd xmm2,xmm3 // tmp /= n

    mov rcx, 2
    cvtsi2sd xmm10 ,rcx // for the division in the formula: (b−a)·(f(b)+f(a))/2
    pxor xmm9, xmm9 // sumResult = 0
    movsd xmm5, xmm0 // init right=a

    loop:
        movsd xmm4, xmm5 // left_new = right_old

        movsd xmm5, xmm4 // right_new = left_new + schrittweite
        addsd xmm5, xmm2

        movsd xmm6, xmm5 // xmm6 = (b-a)
        subsd xmm6, xmm4

        movsd xmm7, xmm5 // xmm7 = f(b)
        mulsd xmm7, xmm7 // x^2

        movsd xmm8, xmm4 // xmm8 = f(a)
        mulsd xmm8, xmm8 // x^2

        addsd xmm7, xmm8 // xmm7 = f(b) + f(a)
        mulsd xmm6, xmm7 // xmm6 = (b-a) * (f(b) + f(a))

        divsd xmm6, xmm10 // xmm6 /= 2
        addsd xmm9, xmm6 // sumResult += xmm6

        cmpsd xmm5, xmm1, 0 // check equality of actual b and current b
        movd rcx, xmm5 // if true, 0xFFFFFFFFH will be stored in rcx, else 00000000H
        cmp rcx, 0
        je loop // repeat if actual b was not yet reached

        movsd xmm0, xmm9
        ret

    nan:
        pxor xmm0, xmm0
        divsd xmm0, xmm0
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
