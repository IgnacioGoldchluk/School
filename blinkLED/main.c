/*Program to change the rate of a blinking LED based on potentiometer analog
 input*/

// Configuration bits
#pragma config FNOSC = FRC
#pragma config IESO = OFF
#pragma config FCKSM = CSECMD
#pragma config OSCIOFNC = OFF
#pragma config POSCMD = XT
#pragma config FWDTEN = OFF
#pragma config JTAGEN = OFF


// Includes
#include <p33fj128gp802.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

#define FCY 32000000UL // FCY to use libpic30 libraries
#include <libpic30.h>

void initADC10b(void);
uint16_t readADC(void);



#define BLINKING_LED LATBbits.LATB0

int main(void)
{
    uint16_t potValue, blinkDelay;
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    // Fcy = 32MHz
    PLLFBD = 50; // M = 52
    CLKDIVbits.PLLPOST=0; // N2 = 2
    CLKDIVbits.PLLPRE=1; // N1 = 3
    
    // Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001);
    // Wait for PLL to lock
    while(OSCCONbits.LOCK!=1) {};
    
    TRISBbits.TRISB0 = 0; // Define RB0 as output
    initADC10b();
    
    // Map ADC value to valid delays
    // 5V, divided by 6k2 + (1k + 10k pot)
    // Max voltage = 3,2V Min voltage = 0,7V
    // 3.2V (992) must map to 20mseg
    // 0.7V (217) must map to 1000mseg
    float m = -1.2645;
    float b = 1274.4;
    
    // Infinite loop
    while(1)
    {
        BLINKING_LED ^=1; // Toggle
        potValue = readADC(); // Read value from ADC manually
        blinkDelay = (uint16_t)(m*potValue + b);
        __delay_ms(blinkDelay);   
    }
    
    return 0;
}

void initADC10b(void)
{
    // Calculated for manual sampling and manual conversion
    /*AD1CON1bits.ADON = 0; // Turn off at first
    AD1CON1bits.AD12B = 0; // 10 bit mode
    AD1CON1bits.FORM = 0b00; // Integer type
    AD1CON1bits.SSRC = 0b00;
    AD1CON1bits.ASAM = 0;
    */
    AD1CON1 = 0;
    
    /*
    AD1CON2bits.VCFG = 0b000;
    AD1CON2bits.CHPS = 0b00;
    */
    AD1CON2 = 0;
    
    // Tcy = 1/32MHz
    AD1CON3bits.ADCS = 9;
    
    AD1PCFGL = 0xFFFE; // Only AN0 as input
    
    AD1CON1bits.ADON = 1; // Enable ADC
}

uint16_t readADC(void)
{
    AD1CON1bits.SAMP = 1; // Start sampling
    __delay_us(20); // 20us delay
    AD1CON1bits.SAMP = 0; // Start conversion
    while(!AD1CON1bits.DONE); // Wait for DONE bit to be set
    return ADC1BUF0;
}
