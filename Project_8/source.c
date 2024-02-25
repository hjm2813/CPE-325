#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void UART_initialize();
char UART_getCharacter();
void UART_sendCharacter(char);
void UART_sendString(char *);
void UART_getLine(char *, int);

void UART_setup(void) {
    P2SEL |= BIT4 + BIT5;   // Set USCI_A0 RXD/TXD to receive/transmit data
    UCA0CTL1 |= UCSWRST;    // Set software reset during initialization
    UCA0CTL0 = 0;           // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;   // Clock source SMCLK
    UCA0BR0 = 0x09;         // 1048576 Hz  / 115200 lower byte
    UCA0BR1 = 0x00;         // upper byte
    UCA0MCTL = 0x02;        // Modulation (UCBRS0=0x01, UCOS16=0)
    UCA0CTL1 &= ~UCSWRST;   // Clear software reset to initialize USCI state machine
    IE2 |= UCA0RXIE;        // Enable USCI_A0 RX interrupt
}



int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    int test = 0;
    char string[50];

    UART_initialize();

    while (1) {
        UART_sendString("Me: ");
        UART_getLine(string, 49);

        if (strcmp(string, "1000") == 0 && test == 1) {
            UART_sendString("Bot: That cannot be true!");
            UART_sendString("\r\n");
        } else if (test == 1) {
            UART_sendString("Bot: You are so young! I am 1.");
            UART_sendString(string);
            UART_sendString("\r\n");
        }
        if (strcmp(string, "Hey, Bot!") == 0) {
            UART_sendString("Bot: Hi, How old are you?");
            UART_sendString("\r\n");
            test = 1;
        } else {
            test = 0;
        }
    }
}

void UART_initialize() {
    UCA0CTL1 |= UCSWRST;
    P2SEL |= BIT4 | BIT5;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 0x09;
    UCA0BR1 = 0x00;
    UCA0MCTL = 0x02;
    UCA0CTL1 &= ~UCSWRST;
}

char UART_getCharacter() {
    while (!(IFG2 & UCA0RXIFG));
    return UCA0RXBUF;



}

void UART_sendCharacter(char s) {
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = s;
}

void UART_sendString(char *string) {
    int length = strlen(string);
    int i;
    for (i = 0; i < length; i++) {
        UART_sendCharacter(string[i]);
    }
}

void UART_getLine(char *buffer, int limit) {
    int i = 0;
    char s;

    while (i < limit - 1) {
        s = UART_getCharacter();
        UART_sendCharacter(s);
        buffer[i] = s;
        i++;

        if (s == '\r') {
            break;
        }

    }

    buffer[i] = '\0';
    UART_sendString("\r\n");
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void) {
    char receivedChar = UART_getCharacter();
    UART_sendCharacter(receivedChar);
    P5OUT ^= BIT1;
}
