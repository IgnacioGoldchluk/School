/*Programa para testear los tiempos y la funcionalidad del ADC
 y el DMA*/


// Configuration bits
#pragma config FNOSC = FRC
#pragma config IESO = OFF
#pragma config FCKSM = CSECMD
#pragma config OSCIOFNC = OFF
#pragma config POSCMD = XT
#pragma config FWDTEN = OFF
#pragma config JTAGEN = OFF

#include <p33fj128gp802.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>


// Test header libraries 
#include "ADC_test.h"
#include "DMA_test.h"

#define BUFFER_PIN LATBbits.LATB3 // Pin para contar cuanto tarda en LLENARSE el bufer
#define DMA_PIN LATBbits.LATB4 // Pin para contar cuanto tarda en TRANSFERIRSE el bufer

void copyBuffer(int16_t *,int16_t *, uint16_t);

extern uint8_t DMABuffer;
extern uint8_t fillFlag;

extern int16_t BufferA[NUMSAMPLES];
extern int16_t BufferB[NUMSAMPLES];

/*/
// Clock configuration, change to #pragma
    _FOSCSEL(FNOSC_FRC & IESO_OFF); // Internal FRC
    _FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT); // Clock switch enabled
                                                    // primary oscillator XT
    
    _FWDT(FWDTEN_OFF);                              // Watchdog timer disabled
    //_FPOR(PPWRT_PWR128);                            // Power up timer enabled 128ms
    _FICD(JTAGEN_OFF);
*/

int main(void)
{   
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
    
       
    TRISBbits.TRISB3 = 0; // Set RB3 as output
    TRISBbits.TRISB4 = 0; // Set RB4 as output
    DMA_PIN = 0;
    int16_t testBuffer[NUMSAMPLES];
    
    initADC1();
    initDMA0();
    
    while(1)
    {
        if(fillFlag)
        {
            DMA_PIN ^=1;
            BUFFER_PIN = 1;
            if(DMABuffer)
                copyBuffer(testBuffer, BufferA, NUMSAMPLES);
            else
                copyBuffer(testBuffer, BufferB, NUMSAMPLES);
            fillFlag = 0;
            BUFFER_PIN = 0;
        }
    }
    return 0;
}


void copyBuffer(int16_t* dest, int16_t* source,uint16_t nSamples)
{
    uint16_t i = nSamples;
    
    while(i-->0)
        *dest++ = *source++;
}
