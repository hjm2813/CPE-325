; Include device header file
.cdecls C, LIST, "msp430.h"

; Constants
SW1_PIN     equ     BIT1; SW1 connected to P1.1
SW2_PIN     equ     BIT2; SW2 connected to P1.2
LED1_PIN    equ     BIT0; LED1 connected to P1.0
LED2_PIN    equ     BIT6; LED2 connected to P1.6

; Variables
LED1_STATE  equ     0; LED1 state : OFF
LED2_STATE  equ     1; LED2 state : ON

; ------------------------------------------------------------------------------ -
.data
; Variables to track LED states
led1_state.byte   LED1_STATE
led2_state.byte   LED1_STATE

; ------------------------------------------------------------------------------ -
.text
RESET :
mov.w   #__STACK_END, SP; Initialize stack pointer
mov.w   #WDTPW | WDTHOLD, & WDTCTL; Stop the watchdog timer

; Initialize ports
bis.b   #SW1_PIN | SW2_PIN, & P1DIR; Set SW1and SW2 as inputs
bis.b   #LED1_PIN | LED2_PIN, & P1DIR; Set LED1and LED2 as outputs
xor .b   #LED1_PIN | LED2_PIN, & P1OUT; Turn off both LEDs initially

; Enable pull - up resistors for switches
bis.b   #SW1_PIN | SW2_PIN, & P1REN

; Configure interrupts for SW1and SW2
bis.b   #SW1_PIN, & P1IE; Enable interrupt for SW1
xor .b   #SW1_PIN, & P1IES; Trigger on falling edge for SW1
bis.b   #SW2_PIN, & P1IE; Enable interrupt for SW2
xor .b   #SW2_PIN, & P1IES; Trigger on falling edge for SW2

main_loop :
; Main program loop
nop
nop
nop
nop
jmp     main_loop

; SW1 interrupt service routine
SW1_ISR :
xor .b   #LED1_PIN, & P1OUT; Toggle LED1
reti

; SW2 interrupt service routine
SW2_ISR :
xor .b   #LED2_PIN, & P1OUT; Toggle LED2

; Update LED2 state
mov.b& led2_state, r15
xor .b   #1, r15; Toggle LED2 state
mov.b   r15, & led2_state

reti

; ------------------------------------------------------------------------------ -
; Stack Pointer definition
; ------------------------------------------------------------------------------ -
.global __STACK_END
.sect.stack

; ------------------------------------------------------------------------------ -
; Interrupt Vectors
; ------------------------------------------------------------------------------ -
.sect   ".reset"; MSP430 RESET Vector
.short  RESET

.sect   ".text:SW1_ISR"; SW1 interrupt vector
.short  SW1_ISR

.sect   ".text:SW2_ISR"; SW2 interrupt vector
.short  SW2_ISR
