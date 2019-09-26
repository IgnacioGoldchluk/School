
#include "ADC_test.h"


void initADC1(void)
{
    // Clock aprox 39MHz, dividir por 5 y samplear a 460ksps porque no se puede mas  
    // Pagina 48 del manual de referencia del ADC para setear los config bits
    // en el orden correcto
    
    AD1CON1bits.ADON    = 0;     // Disable ADC
    AD1CON1bits.AD12B   = 1;     // Select 12 bits ADC
    AD1CON1bits.FORM    = 0b01;  // Signed integer type (must be like this because
                                 // this is how 16 bit WAV works only)
    AD1CON1bits.SSRC    = 0b111; // Internal clock, autoconvert
    AD1CON1bits.ASAM    = 1;     // Autosample
    AD1CON1bits.ADDMABM = 1;     // Write in order of conversion
    
    AD1PCFGL = 0xFFFE; // Only AN0 is analog port
    
    // CSCNA = 0 no scan, BUFS = 0, SMPI = 0000 increment every sample, BUFM = 0
    // ALTS = 0, VCFG = 000 reference AVDD-AVSS (3.3V-0V)
    AD1CON2bits.SMPI = 0;
    
    AD1CON3bits.ADRC = 0; // Clock derived from system clock
    AD1CON3bits.SAMC = 0b00001; // Conversion time = 1*Tad
    
    // Tad = Tcy*(ADCS+1),  Tad = 32MHz/(3+1) = 8MHz
    // Sample rate = 17*Tad => 470ksps aprox
    AD1CON3bits.ADCS = 3; 
    
    /* NOT USED BECAUSE OF INDIRECT MODE
    AD1CON4bits.DMABL = 0b111; // Allocate 128 words per bufer (only 1 bufer used, 256 bytes)
    */
    
    AD1CHS0bits.CH0NA = 0; // Negative input is VREF- (0V)
    AD1CHS0bits.CH0SA = 0; // Positive input is AN0
    
    IFS0bits.AD1IF = 0; // Clear interrupt flag bit
    IEC0bits.AD1IE = 0; // Do not enable interrupts by ADC
    
    // Finally, enable ADC
    AD1CON1bits.ADON = 1; 
}
