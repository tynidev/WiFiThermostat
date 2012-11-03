/*
 * utils.c
 *
 *  Created on: Oct 8, 2012
 *      Author: Tyler
 */

void delay_ms(unsigned int ms)
{
	while (ms != 0)
	{
		ms -= 1;
		__delay_cycles( SYS_CLK / 1000 ); // set for 16Mhz change it to 1000 for 1 Mhz
	}
}
