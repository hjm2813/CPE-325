/*-------------------------------------------------------------------------
* File: Lab10_02.c
* Description: Modify the program from part 1 (do not remove any existing
functionality), so that it calculates the magnitude using data from all
three axes: 𝑀 = √𝑥2 + 𝑦2 + 𝑧2.
* Author(s): Min Han
* Date: 10.23.23
*-----------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>
#include <math.h>
#define BAUDRATE_DIVIDER 104 // Corresponds to 115200 bps at 16MHz SMCLK
#define SW2 (P1IN & BIT3) // Assuming SW2 is on P1.3 and is active low
void UARTInit() {
 UCA0CTL1 |= UCSWRST; // Reset USCI_A0
 UCA0CTL1 |= UCSSEL_2; // Use SMCLK
 UCA0BR0 = BAUDRATE_DIVIDER & 0xFF;
 UCA0BR1 = BAUDRATE_DIVIDER >> 8;
 UCA0MCTL |= UCBRS_0; // No modulation
 UCA0CTL1 &= ~UCSWRST; // Release USCI_A0 reset
}
void UARTSendChar(char c) {
 while(!(UCA0IFG & UCTXIFG)); // Wait until buffer is empty
 UCA0TXBUF = c;
}
void UARTSendString(const char* str) {
 while(*str) {
 UARTSendChar(*str++);
 }
}
int main(void) {
 WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
 BCSCTL1 = CALBC1_16MHZ; // Set DCO to 16MHz
 DCOCTL = CALDCO_16MHZ;
 P1DIR |= BIT0; // Set P1.0 as output - LED2
 P1OUT &= ~BIT0; // Turn off LED2 to start
 P1DIR &= ~BIT3; // Set SW2 as input
 P1REN |= BIT3; // Enable pull-up resistor for SW2
 P1OUT |= BIT3; // Set the resistor to pull-up mode
 UARTInit();
 ADC10CTL1 = INCH_4; // Start with A4 -x axis
 ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;
 while(1) {
 ADC10CTL0 |= ENC + ADC10SC; // Start conversion
 __bis_SR_register(CPUOFF + GIE); // Sleep until ADC interrupt
 float xValue = ADC10MEM * (3.3/1023.0) / 0.3; // Convert to g
 ADC10CTL1 = INCH_5; // Switch to A5 -y axis
 ADC10CTL0 |= ENC + ADC10SC;
 __bis_SR_register(CPUOFF + GIE);
 float yValue = ADC10MEM * (3.3/1023.0) / 0.3;
 ADC10CTL1 = INCH_6; // Switch to A6 - z axis
 ADC10CTL0 |= ENC + ADC10SC;
 __bis_SR_register(CPUOFF + GIE);
 float zValue = ADC10MEM * (3.3/1023.0) / 0.3;
 char buffer[100];
 sprintf(buffer, "X: %f g\n", xValue);
 UARTSendString(buffer);
 sprintf(buffer, "Y: %f g\n", yValue);
 UARTSendString(buffer);
 sprintf(buffer, "Z: %f g\n", zValue);
 UARTSendString(buffer);
 float magnitude = sqrt(xValue*xValue + yValue*yValue + zValue*zValue);
 if(magnitude >= 2) {
 // Turn on LED2 and flash at 2Hz -p1.0
 P1OUT ^= BIT0;
 __delay_cycles(500000); // 0.5 seconds delay
 if(!SW2) { //0 when pressed
 __delay_cycles(4800000); // Wait for 3 seconds
 if(!SW2) { // Check again
 P1OUT &= ~BIT0; // Turn off LED2
 }
 }
 }
 __delay_cycles(160000); // Wait 100ms (10 times a second)
 }
 return 0;
}
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
 __bic_SR_register_on_exit(CPUOFF);
}
