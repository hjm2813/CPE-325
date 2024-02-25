/*-------------------------------------------------------------------------
* File: Project.C
* Description: This C code will let MSP430 be as real timer.
* Author(s): Min Han
* Date: 11.15.23
*-----------------------------------------------------------------------*/
mov
#include <msp430.h>
#define LED1 BIT0 // P1.0
#define LED2 BIT7 // P4.7
#define SW1 BIT1 // P2.1
#define SW2 BIT1 // P1.1
volatile unsigned int switch_count = 0;
volatile unsigned int seconds_elapsed = 0;
void initialize_ports_and_timer(void);
void blink_leds(unsigned int times);
void start_timer(void);
void stop_timer(void);
int main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
initialize_ports_and_timer();
__enable_interrupt();
blink_leds(2); // Initial blink, slower
P1OUT &= ~LED1; // Turn off LED1 immediately after blinking
P4OUT &= ~LED2; // Turn off LED2 immediately after blinking
while (1) {
// Check for switch 1 press
if (!(P2IN & SW1)) {
int i;
for (i = 20000; i > 0; i--); // Debouncing delay
if (!(P2IN & SW1)) { // Confirm Switch 1 is still pressed
switch_count++;
start_timer();
P1OUT |= LED1; // Turn on LED1
P4OUT |= LED2; // Turn on LED2
while (!(P2IN & SW1)); // Wait for switch release
}
}
// Check for switch 2 press
if (!(P1IN & SW2)) {
int i;
for (i = 20000; i > 0; i--); // Debouncing delay
if (!(P1IN & SW2)) { // Confirm Switch 2 is still pressed
stop_timer();
switch_count = 0;
P1OUT &= ~LED1; // Turn off LED1
P4OUT &= ~LED2; // Turn off LED2
while (!(P1IN & SW2)); // Wait for switch release
}
}
}
}
void initialize_ports_and_timer(void) {
P1DIR |= LED1; // Set LED1 as output
P4DIR |= LED2; // Set LED2 as output
P1DIR &= ~SW2; // Set P1.1 as input for SW2
P1REN |= SW2; // Enable pull-up resistor for SW2
P1OUT |= SW2; // Set pull-up for SW2
P2DIR &= ~SW1; // Set P2.1 as input for SW1
P2REN |= SW1; // Enable pull-up resistor for SW1
P2OUT |= SW1; // Set pull-up for SW1
TA0CCTL0 = CCIE; // Enable Timer interrupt
TA0CCR0 = 32768 - 1; // 1-second interval (assuming ACLK at 32768 Hz)
TA0CTL = TASSEL_1 + MC_1; // ACLK as the source, Up mode
}
void blink_leds(unsigned int times) {
unsigned int i;
for (i = 0; i < times; i++) {
P1OUT |= LED1; // Turn on LED1
P4OUT |= LED2; // Turn on LED2
__delay_cycles(500000); // Slower blinking
P1OUT &= ~LED1; // Turn off LED1
P4OUT &= ~LED2; // Turn off LED2
__delay_cycles(500000); // Delay between blinks
}
}
void start_timer(void) {
TA0CTL |= TACLR; // Clear Timer_A counter
TA0CTL |= MC_1; // Start Timer_A in up mode
seconds_elapsed = 0; // Reset seconds counter
}
void stop_timer(void) {
TA0CTL &= ~MC_1; // Stop Timer_A
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void) {
seconds_elapsed++;
if (seconds_elapsed >= 60 * switch_count) {
blink_leds(switch_count); // Blink LEDs as per the switch count
P1OUT |= LED1; // Ensure LED1 stays on after blinking
P4OUT |= LED2; // Ensure LED2 stays on after blinking
seconds_elapsed = 0; // Reset seconds count
stop_timer(); // Stop the timer until the next button press
