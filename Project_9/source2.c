/*-------------------------------------------------------------------------
* File: Lab09_02
* Description: It uses SPI communication to receive commands and updates
from another MSP430 device.
* Author(s): Min Han
* Date: 10.18.23
*-----------------------------------------------------------------------*/


#include <msp430.h>
#include <msp430xG46x.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
unsigned char dutyCycle = 25;
unsigned char blinkCount = 0;
char resetBlinkCount = 0;
void SPI_setup(void) {
 BCSCTL1 = CALBC1_1MHZ;
 DCOCTL = CALDCO_1MHZ;
 P1SEL |= BIT1 + BIT2;
 UCB0CTL1 = UCSSEL_2;
 UCB0BR0 = 0x08;
 UCB0BR1 = 0;
 UCB0CTL0 = UCMST + UCSYNC;
 UCB0CTL1 |= UCSSEL_2;
 UCB0CTL1 &= ~UCSWRST;
}
void LED_setup(void) {
 P1DIR |= BIT0;
}
void main(void) {
 WDTCTL = WDTPW + WDTHOLD;
 SPI_setup();
 LED_setup();
 __bis_SR_register(GIE);
 while (1) {
 if (resetBlinkCount) {
 blinkCount = 0;
 resetBlinkCount = 0;
 }
 if (dutyCycle > 0) {
 P1OUT |= BIT0;
 delayVariable(1000000 / dutyCycle);
 P1OUT &= ~BIT0;
 delayVariable(1000000 * (100 - dutyCycle) / dutyCycle);
 blinkCount++;
 if (blinkCount > 127) {
 blinkCount = 127;
 }
 }
 }
}
void delayVariable(unsigned long cycles) {
 while(cycles--) {
 __delay_cycles(1);
 }
}
#pragma vector = USI_VECTOR
__interrupt void SPI_ISR(void) {
 char receivedData = UCB0RXBUF;
 if (receivedData == 200) {
 resetBlinkCount = 1;
 }
 else if (receivedData == 255) {
 UCB0TXBUF = blinkCount;
 }
 else {
 dutyCycle = receivedData;
 }
}
