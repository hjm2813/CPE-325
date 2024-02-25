/*-------------------------------------------------------------------------
* File: Lab10_01.c
* Description: This program should sample x, y and z axes 10-times per
second.
* Author(s): Min Han
* Date: 10.23.23
*-----------------------------------------------------------------------*/
#include <msp430xG46x.h>
#include <stdio.h>
#define BAUDRATE_DIVIDER 104 // Corresponds to 115200 bps at 16MHz SMCLK
void UARTInit() {
 UCA0CTL1 |= UCSWRST; // Reset USCI_A0
 UCA0CTL1 |= UCSSEL_2; // Use SMCLK
 UCA0BR0 = BAUDRATE_DIVIDER & 0xFF;
 UCA0BR1 = BAUDRATE_DIVIDER >> 8;
 UCA0MCTL |= UCBRS_0; // No modulation
 UCA0CTL1 &= ~UCSWRST; // Release USCI_A0 reset
}
void UARTSendChar(char c) {
 while(!(UCA0IFG & UCTXIFG)); // Wait until buffer be empty
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
 UARTInit();
 ADC10CTL1 = INCH_4; // Start with A4 -x axis
 ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;
 while(1) {
 ADC10CTL0 |= ENC + ADC10SC; // Start conversion
 __bis_SR_register(CPUOFF + GIE); // Sleep until ADC interrupt
 float xValue = ADC10MEM * (3.3/1023.0) / 0.3;
 ADC10CTL1 = INCH_5; // Switch to A5 - y- axis
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
 __delay_cycles(160000); // Wait 100ms (10 times a second)
 }
 return 0;
}
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
 __bic_SR_register_on_exit(CPUOFF);
}
