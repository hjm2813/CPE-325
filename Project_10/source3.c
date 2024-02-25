/*-------------------------------------------------------------------------
* File: Lab10_03.c
* Description: This is a C program that outputs different waveforms using
DAC.
* Author(s): Min Han
* Date: 10.23.23
*-----------------------------------------------------------------------*/
#include <msp430.h>
#include "triangular_wave.h"
#include "sine_wave.h"
#define SW1 BIT0
#define SW2 BIT1
uint16_t current_amplitude_multiplier = 1;
uint16_t* current_wave = triangular_wave;
void main(void)
{
 WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
 while(1)
 {
 if(SW1_PRESSED)
 current_wave = sine_wave;
 if(SW2_PRESSED)
 current_amplitude_multiplier = 2;
 else
 current_amplitude_multiplier = 1;
 for(int i=0; i<512; i++)
 {
 DAC_OUTPUT = current_wave[i] * current_amplitude_multiplier;
 __delay_cycles(39062);
 }
 }
}
Table ##: Program 4 source code
/*-------------------------------------------------------------------------
* File: Lab10_03.m
* Description: These LUTs will be included as header files that are
utilized by your main C program.
* Author(s): Min Han
* Date: 10.23.23
*-----------------------------------------------------------------------*/
% Sample count
N = 512;
% Time vector
t = linspace(0, 1, N); % One period (1/50Hz = 20ms)
% Triangular Waveform
triangular_wave = 2*(abs(2*(t - floor(t + 0.5))) - 0.5);
triangular_wave = triangular_wave - min(triangular_wave);
triangular_wave = (triangular_wave / max(triangular_wave)) * 4095; %
Normalize to 12-bit DAC (0 to 4095)
% Sine Waveform
sine_wave = sin(2 * pi * 50 * t);
sine_wave = sine_wave * 2047.5 + 2047.5; % Offset to center and scale for
12-bit DAC
% Save as header files
fid = fopen('triangular_wave.h', 'w');
fprintf(fid, 'uint16_t triangular_wave[%d] = {%d', N, triangular_wave(1));
fprintf(fid, ',%d', triangular_wave(2:end));
fprintf(fid, '};\n');
fclose(fid);
fid = fopen('sine_wave.h', 'w');
fprintf(fid, 'uint16_t sine_wave[%d] = {%d', N, sine_wave(1));
fprintf(fid, ',%d', sine_wave(2:end));
fprintf(fid, '};\n');
fclose(fid);
