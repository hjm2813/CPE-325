/*-------------------------------------------------------------------------
 * File:  Lab_06_1.asm
 * Description: An assembly program that interfaces switches, SW1 and SW2, and LEDs, LED1 and
LED2, as following instructions.
.
 * Author(s):   Min Han
 * Date:       09.25.23
 *-----------------------------------------------------------------------*/
.cdecls C,LIST,"msp430.h"

; Define RESET and ISR for linker
	.def RESET
	.def ISR_SW1
	.def ISR_SW2

.text
	.retain
	.retainrefs

RESET:
    mov.w #__STACK_END, SP
    mov.w   #WDTPW|WDTHOLD,&WDTCTL

Setup:
    bis.b #0x04, &P2DIR       ; Set P2.2 (LED2) to output direction
    bic.b #0x04, &P2OUT       ; Ensure LED2 is off

    bis.b #0x01, &P1IE        ; Enable interrupt on P1.0 (SW1)
    bic.b #0x01, &P1IES       ; Set to trigger on the rising edge of P1.0
    bic.b #0x01, &P1IFG       ; Clear P1.0 interrupt flag

    bis.b #0x02, &P1IE        ; Enable interrupt on P1.1 (SW2)
    bic.b #0x02, &P1IES       ; Set to trigger on the rising edge of P1.1
    bic.b #0x02, &P1IFG       ; Clear P1.1 interrupt flag

    bis.w #GIE, SR            ; Enable Global Interrupts

InfLoop:
    jmp $
	nop
; Stack Pointer definition
	.global __STACK_END
	.sect .stack

; Interrupt Vectors
	.sect ".reset"
	.short RESET

	.sect ".int20"
	.short ISR_SW1

	.sect ".int21"
	.short ISR_SW2

; SW1 Interrupt Service Routine
ISR_SW1:
    ; SW1 is pressed, turn on LED1
    bis.b #0x01, &P2OUT
    reti

; SW2 Interrupt Service Routine (toggle LED2)
ISR_SW2:
    ; Check if SW2 is pressed (P1.1 is low)
    bit.b #0x02, &P1IN
    jz SW2_PRESSED

    ; SW2 is released, turn off LED2
    bic.b #0x04, &P2OUT
    reti

SW2_PRESSED:
    ; Toggle LED2 state
    xor.b #0x04, &P2OUT
    reti

