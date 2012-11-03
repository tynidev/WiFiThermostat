#include <msp430.h>

#include "lm35.h"
#include "adc.h"

float lm35_tempF(float vref, unsigned int channel)
{
  return (((float)adc_read(channel) * vref * 100 * 1.8) / 1024.0) + 32;
}

float lm35_tempC(float vref, unsigned int channel)
{
  return ((float)adc_read(channel) * vref * 100) / 1024.0;
}
