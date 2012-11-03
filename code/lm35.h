#ifndef _LM35_H_
#define _LM35_H_

float lm35_tempF(float vref, unsigned int channel);
float lm35_tempC(float vref, unsigned int channel);

#endif /* _LM35_H_ */
