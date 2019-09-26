#include "DMA_test.h"

int16_t BufferA[NUMSAMPLES] __attribute__((space(dma)));
int16_t BufferB[NUMSAMPLES] __attribute__((space(dma)));


void initDMA0(void)
{
    // Word length, 512 words (1024 bytes) to write
    
    // LEER PAGINA 50 DEL ADC PARA CONFIGURAR TODO
    
    DMA0CONbits.CHEN = 0; // Disable DMA just in case
    
    DMA0CONbits.SIZE  = 0;    // 16 bit words
    DMA0CONbits.DIR   = 0;    // Read from peripheral, store in RAM
    
    DMA0CONbits.AMODE = 0b00; // Register indirect mode, IMPORTANT BECAUSE
                              // IT ALLOWS TO WRITE MORE THAN 128 WORDS IN THE
                              // ADC BUFFER
    
    DMA0CONbits.MODE  = 0b10; // Continuous mode, ping-pong enabled
    
    DMA0REQbits.IRQSEL = 13; // Interrupt source = ADC1
    
    DMA0PAD = (volatile int16_t)&ADC1BUF0; //Read from ADC buffer
    
    // Buffers defined in main,
    // TO DO, ADD THE ALIGMENT IF THERE'S ANY
    DMA0STA = __builtin_dmaoffset(BufferA);
    DMA0STB = __builtin_dmaoffset(BufferB);
    
    // NUMSAMPLES = 512, Buffer will store 512 conversions
    // at 470ksps aprox
    DMA0CNT = NUMSAMPLES-1;
    
    IFS0bits.DMA0IF  = 0; // Clear the DMA interrupt flag
    IEC0bits.DMA0IE  = 1; // Enable interrupts by DMA
    
    DMA0CONbits.CHEN = 1; // Enable DMA
}

// Variable declaration to handle buffers and interrupts outside the routine
uint8_t DMABuffer = 0; // DMABuffer = 0? fill bufferA, DMABuffer = 1? fill bufferB
uint8_t fillFlag  = 0;

// DMA0 Interruption Routine
void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
    DMABuffer      ^=1; // Toggle Buffer 
    fillFlag        = 1; // Set flag
    IFS0bits.DMA0IF = 0; // Clear Interrupt Flag
}
