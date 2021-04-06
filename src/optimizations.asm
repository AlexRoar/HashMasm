; =================================================================
;	                System V calling convention
; =================================================================
; |	 		edi  esi  edx  ecx  r8d  r9d | stack -->
; |  		 1    2    3    4    5    6    7     ...
; | 64bit:	rdi  rsi  rdx  rcx  r8   r9
; | 		r10 - function's static chain pointer
; |
; |  		Floating point:
; |  		xmm0 xmm1 xmm2 xmm3 xmm4 xmm5 xmm6 xmm7 | stack -->
; |  		 1    2    3    4    5    6    7    8     9     ...
; |
; |         Clean-up: Caller
; |			Must preserve: rsp, rbp, rbx, r12 - r15
; |
; |			128 bytes stack zone is red-zonne
; |
; |			Can be used: rax, 1st return register
; |						 rdx, 2nd return register
; | 					 r11, tmp reg
; =================================================================
%define ArgNo(n) n * 8 + 16
IntSize 	equ 32
ByteSize 	equ 8
FNV64Const  equ 1099511628211
global      _fnv64
global      _hash32Asm

section  .text
_fnv64: ; size_t fnv64(const char *p, size_t hash = 0)
    mov rax, rsi
    mov rcx, FNV64Const
.loop:
    movzx  rdx, byte [rdi]
    inc    rdi
    imul   rax, rcx
    xor    rax, rdx
    test   dl, dl
    jne .loop
    ret

_hash32Asm: ; size_t hash32Asm(const unsigned char *p, size_t len, size_t hash = 0)
    mov rax, rdx
    mov r8, rsi
    and r8, 0x3F ; 64 - 1
    sub rsi, r8

    test r8, r8
    je .marginLoopEnd
    .marginLoop:
        crc32 rax, byte [rdi]
        inc rdi
        dec r8
        test r8, r8
        jne .marginLoop
    .marginLoopEnd:
    cmp rsi, 0
    jle .loopEnd
    .loop:
        crc32 rax, qword [rdi]
        add   rdi, 8
        sub   rsi, 8
        test  rsi, rsi
        jne .loop
    .loopEnd:
    ret