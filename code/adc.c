#include <msp430.h>

#include "adc.h"

unsigned int adc_read(unsigned int channel)
{
  ADC10CTL0 &= ~ENC; 				/* Disable ADC */
  ADC10CTL0 = ADC10SHT_3 + ADC10ON; 		/* 16 clock ticks, 
                                                   ADC On */
  ADC10CTL1 = ADC10SSEL_3 + channel; 		/* Set 'chan', SMCLK */
  ADC10CTL0 |= ENC + ADC10SC; 			/* Enable and start 
                                                   conversion */
  while (!(ADC10CTL0 & ADC10IFG));		/* wait */
  
  return ADC10MEM;
}
