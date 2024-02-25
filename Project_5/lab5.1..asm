	.cdecls C, LIST, "msp430.h"     ; Include device header file

;-------------------------------------------------------------------------------
	.def    RESET                   ; Export program entry-point to

;-------------------------------------------------------------------------------
.data                           ; Data section (for constants and variables)
; Define constants
m           .word    0
c           .word    0
ARRAY_SIZE  .equ    10
X           .space  ARRAY_SIZE ; Array X
result      .word   0

;-------------------------------------------------------------------------------
.text                           ; Code section (actual program code)
RESET:  mov.w   #__STACK_END,SP ; Initialize stack pointer
        mov.w   #WDTPW|WDTHOLD,&WDTCTL ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
    ; Initialize X array with user-defined values


    mov.w   #X, R4            ; Load address of X into R4
    mov.w   #ARRAY_SIZE, R5   ; Load size of X array into R5

    ; Input values from the user
    ; Input value for X[0] from the user

    ; Assume the user input is stored in R6
    mov.w   R6, (R4)           ; Store it in X[0]

    ; Input value for X[1] from the user
    ; Assume the user input is stored in R7
    mov.w   R7, 2(R4)          ; Store it in X[1]

    mov.w   #3, m             ; Set the value of 'm'
    mov.w   #5, c             ; Set the value of 'c'

    ; Call the SW_linear subroutine
    mov.w   R5, R12            ; Pass the size of X as an argument
    mov.w   R4, R13            ; Pass the address of X as an argument
    mov.w   R15, R14           ; Pass the address of 'result' variable as an argument
    call    SW_linear          ; use SW_linear
    mov.w   result, R15        ; Store the result in the 'result' variable

    ; Call the HW_linear subroutine
    mov.w   R5, R12            ; Pass the size of X as an argument
    mov.w   R4, R13            ; Pass the address of X as an argument
    mov.w   R15, R14           ; Pass the address of 'result' variable as an argument
    call    HW_linear          ; use HW_linear
    mov.w   result, R15        ; Store the result in the 'result' variable

    ; Display the results

end:
    ; program's end

; SW_linear subroutine
SW_linear:
    push    R12                ; Save registers
    push    R13
    push    R14

    mov.w   R13, R4            ; Load 'X' address into R4
    mov.w   R12, R5            ; Load 'm' (size of X) into R5
    mov.w   #0, R15            ; Initialize result

multiply_loop_sw:
    mov.w   @R4+, R12          ; Load X[i] into R12 (X[i] = X[i-1])
    mov.w   R12, OP2           ; Set the second operand for hardware multiplication
    MPY     R12, R5, R12       ; Hardware multiply X[i] by m
    add.w   R12, R15           ; Add the result to R15 (result)
    dec.w   R5                 ; Decrement loop counter
    jnz     multiply_loop_sw    ; Repeat until R5 is zero

    add.w   c, R15             ; Add 'c' to the result
    mov.w   @R14, R15         ; Store the result in the 'result' variable
    pop     R14                ; Restore registers
    pop     R13
    pop     R12
    ret

; HW_linear subroutine
HW_linear:
    push    R12                ; Save registers
    push    R13
    push    R14

    mov.w   R13, R4            ; Load 'X' address into R4
    mov.w   R12, R5            ; Load 'm' (size of X) into R5
    mov.w   #0, R15            ; Initialize result

multiply_loop_hw:
    mov.w   @R4+, R12          ; Load X[i] into R12 (X[i] = X[i-1])
    mov.w   R12, OP2           ; Set the second operand for hardware multiplication
    MPY     R12, R5, R12       ; Hardware multiply X[i] by m
    add.w   R12, R15           ; Add the result to R15 (result)
    dec.w   R5                 ; Decrement loop counter
    jnz     multiply_loop_hw    ; Repeat until R5 is zero

    add.w   c, R15             ; Add 'c' to the result
    mov.w   @R14, R15        	 ; Store the result in the 'result' variable
    pop     R14                ; Restore registers
    pop     R13
    pop     R12
    ret

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
		.global __STACK_END
		.sect   .stack
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
		.sect   ".reset"                ; MSP430 RESET Vector
		.short  RESET
.end