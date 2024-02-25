	.cdecls C, LIST, "msp430.h"     ; Include device header file

;-------------------------------------------------------------------------------
		.def    RESET                   ; Export program entry-point to

;-------------------------------------------------------------------------------
.data                           ; Data section (for constants and variables)
; Define constants
m           .word    0
c           .word    0
ARRAY_SIZE  .equ    8
array1      .space  ARRAY_SIZE * 2 ; Array 1 (16-bit elements)
array2      .space  ARRAY_SIZE * 2 ; Array 2 (16-bit elements)
result_sw   .word   0            ; Software Dot Product Result
result_hw   .word   0            ; Hardware Dot Product Result

;-------------------------------------------------------------------------------
.text                           ; Code section 
RESET:  mov.w   #__STACK_END,SP ; Initialize stack pointer
        mov.w   #WDTPW|WDTHOLD,&WDTCTL ; Stop watchdog timer

main:
    ; Initialize array1 and array2 with user-defined values

    mov.w   #array1, R4         ; Load address of array1 into R4
    mov.w   #ARRAY_SIZE, R5     ; Load size of array1 into R5
    mov.w   #array2, R6         ; Load address of array2 into R6

    ; Input values for array1
    call    InputArray1          ; Call subroutine to input array1 values

    ; Input values for array2
    call    InputArray2          ; Call subroutine to input array2 values

    ; Input values for 'm' and 'c' from the user
    ; Assume the user input for 'm' is stored in R10
    ; Assume the user input for 'c' is stored in R11
    mov.w   R10, m              ; Set the value of 'm' from user input
    mov.w   R11, c              ; Set the value of 'c' from user input

    ; Call the SW_dot_product subroutine
    mov.w   #ARRAY_SIZE, R12    ; Pass the size of the arrays as an argument
    mov.w   #array1, R13        ; Pass the address of array1 as an argument
    mov.w   #array2, R14        ; Pass the address of array2 as an argument
    mov.w   #result_sw, R15     ; Pass the address of result_sw as an argument
    call    SW_dot_product      ; Calculate dot product using software

    ; Call the HW_dot_product subroutine
    mov.w   #ARRAY_SIZE, R12    ; Pass the size of the arrays as an argument
    mov.w   #array1, R13        ; Pass the address of array1 as an argument
    mov.w   #array2, R14        ; Pass the address of array2 as an argument
    mov.w   #result_hw, R15     ; Pass the address of result_hw as an argument
    call    HW_dot_product      ; Calculate dot product using hardware

    ; Display the results

end:


; SW_dot_product subroutine
SW_dot_product:
    push    R12                ; Save registers
    push    R13
    push    R14
    push    R15

    mov.w   R12, R7            ; Load size of the arrays into R7
    mov.w   R13, R4            ; Load address of array1 into R4
    mov.w   R14, R5            ; Load address of array2 into R5
    mov.w   R15, R6            ; Load address of result_sw into R6
    mov.w   #0, R15            ; Initialize result

dot_product_sw_loop:
    mov.w   @R4+, R12          ; Load array1[i] into R12
    mov.w   @R5+, R13          ; Load array2[i] into R13

    ; Multiply array1[i] by array2[i] and accumulate the result
    MPY     R12, R13, R12      ; Hardware multiply array1[i] by array2[i]
    add.w   R12, R15           ; Add the result to R15 (result)

    dec.w   R7                 ; Decrement loop counter
    jnz     dot_product_sw_loop ; Repeat until R7 is zero

    add.w   c, R15             ; Add 'c' to the result
    mov.w   @R6, R15           ; Store the result in result_sw
    pop     R15                ; Restore registers
    pop     R14
    pop     R13
    pop     R12
    ret

; HW_dot_product subroutine
HW_dot_product:
    push    R12                ; Save registers
    push    R13
    push    R14
    push    R15

    mov.w   R12, R7            ; Load size of the arrays into R7
    mov.w   R13, R4            ; Load address of array1 into R4
    mov.w   R14, R5            ; Load address of array2 into R5
    mov.w   R15, R6            ; Load address of result_hw into R6
    mov.w   #0, R15            ; Initialize result

dot_product_hw_loop:
    mov.w   @R4+, R12          ; Load array1[i] into R12
    mov.w   @R5+, R13          ; Load array2[i] into R13

    ; Multiply array1[i] by array2[i] and accumulate the result
    MPY     R12, R13, R12      ; Hardware multiply array1[i] by array2[i]
    add.w   R12, R15           ; Add the result to R15 (result)

    dec.w   R7                 ; Decrement loop counter
    jnz     dot_product_hw_loop ; Repeat until R7 is zero

    add.w   c, R15             ; Add 'c' to the result
    mov.w   @R6, R15           ; Store the result in result_hw
    pop     R15                ; Restore registers
    pop     R14
    pop     R13
    pop     R12
    ret

; InputArray1 subroutine
InputArray1:
    push    R4                 ; Save registers
    push    R5

    mov.w   #array1, R4        ; Load address of array1 into R4
    mov.w   #ARRAY_SIZE, R5    ; Load size of array1 into R5

    ; Input values from the user for array1 using a loop
    mov.w   #0, R6             ; Loop counter
input_array1_loop:
    ; for input and store it in R7

    ; Store the value in the array
    mov.w   @R4+, R7

    inc.w   R6                 ; Increment the loop counter
    cmp.w   R6, R5             ; Compare with the array size
    jnz     input_array1_loop   ; Repeat if not all elements are entered

    pop     R5                 ; Restore registers
    pop     R4
    ret

; InputArray2 subroutine
InputArray2:
    push    R6                 ; Save registers
    push    R7

    mov.w   #array2, R6        ; Load address of array2 into R6
    mov.w   #ARRAY_SIZE, R7    ; Load size of array2 into R7

    ; Input values from the user for array2 using a loop
    mov.w   #0, R8             ; Loop counter
input_array2_loop:
    ; input and store it in R9

    ; Store the value in the array
    mov.w   @R6+, R9

    inc.w   R8                 ; Increment the loop counter
    cmp.w   R8, R7             ; Compare with the array size
    jnz     input_array2_loop   ; Repeat if not all elements are entered

    pop     R7                 ; Restore registers
    pop     R6
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
