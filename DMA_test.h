/* Header DMA.c, prototype and constant declarations*/

#include <stdio.h>
#include <stdlib.h>
#include <p33fj128gp802.h>
#include <stdint.h>
#include <stdbool.h>

#define NUMSAMPLES 512 // Make sure to have enough samples 

void initDMA0(void);
void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void);

