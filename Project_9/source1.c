/*-------------------------------------------------------------------------
* File: Lab09_01.c
* Description: This code allows users to adjust LED blinking frequency and
view the current blink count through UART and SPI communication.
* Author(s): Min Han
* Date: 10.18.23
*-----------------------------------------------------------------------*/
#include <msp430xG46x.h>
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
volatile unsigned int blinkCount = 0;
char responseReceived = 0;
void UART_setup(void) {
 BCSCTL1 = CALBC1_1MHZ;
 DCOCTL = CALDCO_1MHZ;
 P3SEL |= BIT4 + BIT5;
 UCA0CTL1 |= UCSWRST;
 UCA0CTL1 = UCSSEL_2;
 UCA0BR0 = 17;
 UCA0BR1 = 0;
 UCA0MCTL = UCBRS2 + UCBRS0;
 UCA0CTL1 &= ~UCSWRST;
}
void SPI_setup(void) {
 BCSCTL1 = CALBC1_1MHZ;
 DCOCTL = CALDCO_1MHZ;
 P1SEL |= BIT1 + BIT2 + BIT4;
 UCB0CTL1 = UCSSEL_2;
 UCB0BR0 = 8;
 UCB0BR1 = 0;
 UCB0CTL0 = UCMSB + UCMST + UCSYNC;
 UCB0CTL1 &= ~UCSWRST;
}
void sendToMSP430F2013(char data) {
 UCB0TXBUF = data;
 while (!(IFG2 & UCB0RXIFG));
 // Wait until transmission is complete
}
char receiveFromMSP430F2013() {
 UCB0TXBUF = 0; // Send a dummy byte
 while (!(IFG2 & UCB0RXIFG)); // Wait until reception is complete
 return UCB0RXBUF;
}
void main(void) {
 WDTCTL = WDTPW + WDTHOLD; // Stop the watchdog timer
 UART_setup();
 SPI_setup();
 __bis_SR_register(GIE);
 char inputBuffer[5];
 int dutyCycle = 25; // Default duty cycle
 while (1) {
 printf("Enter duty cycle (0-100, ? for current blinks, - for reset): ");
 scanf("%s", inputBuffer);
 if (inputBuffer[0] == '?') {
 sendToMSP430F2013(255);
 responseReceived = 1;
 }
 else if (inputBuffer[0] == '-') {
 sendToMSP430F2013(200);
 printf("Current blinks reset\n");
 }
 else {
 int newDutyCycle = atoi(inputBuffer);
 if (newDutyCycle >= 0 && newDutyCycle <= 100) {
 dutyCycle = newDutyCycle;
 sendToMSP430F2013(dutyCycle);
 }
 else {
 printf("Invalid duty cycle entered\n");
 }
 }
 if (responseReceived) {
 char response = receiveFromMSP430F2013();
 printf("Current blinks: %d\n", response);
 responseReceived = 0;
 }
 }
}
