/*-------------------------------------------------------------------------
 * File:  Lab_06_2.c
 * Description: updated code to meet some requirements
 * Author(s):   Min Han
 * Date:       09.0.23
-------------------------------------------------------------------------*/

#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    long int i = 0;

    P2DIR |= 0x06; // P2.1, P2.2 to output direction
    P2OUT |= 0x02; // Turn on LED2

    unsigned int switchState = 0;

    while (1) {

        unsigned int sw1 = P1IN & BIT0; // SW1 on P1.0
        unsigned int sw2 = P1IN & BIT1; // SW2 on P1.1

        if (sw1 && sw2) { // Both SW1 and SW2 pressed
            switchState = 3; // Set switchState to 3
        } else if (sw1) { // SW1 pressed
            switchState = 1; // Set switchState to 1
        } else if (sw2) { // SW2 pressed
            switchState = 2; // Set switchState to 2
        }

        // Update clock settings based on switchState
        switch (switchState) {
            case 1: // SW1 pressed, change clock to 3MHz
                // Configure clock to 3MHz
                // Example: CSCTL1 = DCOFSEL_0; // Set DCO to 1MHz
                break;
            case 2: // SW2 pressed, reset clock to default 1MHz
                // Configure clock to 1MHz
                // Example: CSCTL1 = DCOFSEL_3; // Set DCO to 8MHz
                break;
            case 3: // Both SW1 and SW2 pressed, change clock to 8MHz
                // Configure clock to 8MHz
                // Example: CSCTL1 = DCOFSEL_6; // Set DCO to 8MHz
                break;
        }

        // Delay for LED blinking
        for (i = 0; i < 50000; i++);

        P2OUT ^= 0x06; // Toggle LED1 and LED2
    }
}
