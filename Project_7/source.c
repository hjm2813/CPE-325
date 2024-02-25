/*-------------------------------------------------------------------------
 * File:  Lab_07_1.c
 * Description: This program works with PWM, controls brightness of the LED
.
 * Author(s):   Min Han
 * Date:       10.02.23
 *-----------------------------------------------------------------------*/
. #include <msp430xG46.h>

#define LED1_PIN BIT2
#define LED2_PIN BIT1
#define SW1_PIN BIT0
#define SW2_PIN BIT1

volatile int brightnessLevel = 0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop the watchdog timer

    P2DIR |= BIT2; // Set LED1 as an output
    P2OUT &= ~BIT2; // Turn off LED1 initially

    P2DIR |= BIT1; // Set LED2 as an output
    P2OUT &= ~BIT1; // Turn off LED2 initially

    P1DIR &= ~(SW1_PIN | SW2_PIN); // Set SW1 and SW2 pins as inputs
    P1OUT |= SW1_PIN | SW2_PIN; // Enable internal pull-up resistors for SW1 and SW2

    P1IE |= SW1_PIN | SW2_PIN; // Enable interrupt for SW1 and SW2
    P1IES |= SW1_PIN | SW2_PIN; // Set interrupt edge to falling edge
    P1IFG &= ~(SW1_PIN | SW2_PIN); // Clear interrupt flags for SW1 and SW2

    // Configure Timer A for PWM
    TACCR0 = 1000; // Set the period
    TACCR1 = 0; // Set the initial duty cycle to 0
    TACCTL1 = OUTMOD_7; // Reset the output when CCR1 is reached
    TACTL = TASSEL_2 + MC_1; // Timer source: SMCLK (1MHz), Mode: Up to CCR0

    __enable_interrupt(); // Enable global interrupts

    while (1)
    {
        __bis_SR_register(LPM0_bits); // Enter low power mode when not processing
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    if (P1IFG & SW1_PIN)
    {
        // Increase brightness level when SW1 is pressed
        if (brightnessLevel < 9)
        {
            brightnessLevel++;
        }
    }
    else if (P1IFG & SW2_PIN)
    {
        // Decrease brightness level when SW2 is pressed
        if (brightnessLevel > 0)
        {
            brightnessLevel--;
        }
    }

    // Update PWM duty cycle based on brightness level
    TACCR1 = brightnessLevel * 100;

    P1IFG &= ~(SW1_PIN | SW2_PIN); // Clear interrupt flags
    __bic_SR_register_on_exit(LPM0_bits); // Exit low power mode
}
