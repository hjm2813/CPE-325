/*-------------------------------------------------------------------------
 * File:  Lab_03_1.c
 * Description: This C program will controls the MSP 430 panel LED lights.
 * Author(s):   Min Han
 * Date:       09.04.23
 *-----------------------------------------------------------------------*/
#include <msp430.h>

#define SW1 BIT0 // P1.0
#define SW2 BIT1 // P1.1
#define LED1 BIT2 // P1.2
#define LED2 BIT3 // P1.3

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    P1DIR |= LED1 + LED2; // Set LED1 and LED2 as outputs
    P1OUT |= LED1; // Turn on LED1
    P1OUT &= ~LED2; // Turn off LED2

    P1DIR &= ~(SW1 + SW2); // Set SW1 and SW2 as inputs
    P1REN |= SW1 + SW2; // Enable pull-up/pull-down resistors for SW1 and SW2
    P1OUT |= SW1 + SW2; // Set pull-up resistor for SW1 and SW2

    while (1)
    {
        if (!(P1IN & SW1)) // Check if SW1 is pressed
        {
            P1OUT &= ~LED1; // Turn off LED1
            // Blink LED2 at 7Hz (approximately 143ms ON, 143ms OFF)
            __delay_cycles(143000);
            P1OUT ^= LED2; // Toggle LED2
            __delay_cycles(143000);
        }
        else if (!(P1IN & SW2)) // Check if SW2 is pressed
        {
            P1OUT |= LED2; // Turn on LED2
            // Blink LED1 at 2Hz (approximately 500ms ON, 500ms OFF)
            __delay_cycles(500000);
            P1OUT ^= LED1; // Toggle LED1
            __delay_cycles(500000);
        }
        else // If no switches are pressed
        {
            P1OUT |= LED1; // Turn on LED1
            P1OUT &= ~LED2; // Turn off LED2
        }
    }
}
