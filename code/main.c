/*
 * main.c
 */

#include <msp430.h>
#include <stdlib.h>

#include "ST7565R.h"
#include "utils.h"
#include "lm35.h"
#include "wifly.h"

char RX_BUF[RX_BUF_SIZE];
char MSG[RX_BUF_SIZE];
volatile int msg_received = 0;

void init()
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;

  wifly_init("TylerN", "DZwV4FGb", 9600);
  ST7565R_Init();
  
  __asm("eint");
}

int main(void)
{
  char temp_str[10] = "         ";

  init();

  LOOP:
  ST7565R_Clear();
  DisplayBox();

  while(!msg_received)
  { // if no message is received just display current temperature
    itoa(lm35_tempF(3.6, 0), temp_str, 10);
    ST7565R_PageAddress(1);
    ST7565R_ColumnAddress(5);  
    ST7565R_WriteString(temp_str);
  }
  
  __asm("dint");
  // if a message has been received then change system state
  
  ST7565R_Clear();
  DisplayBox();
  ST7565R_PageAddress(1);
  ST7565R_ColumnAddress(5);  
  ST7565R_WriteString(MSG);
  
  msg_received = 0;
  __asm("eint");

  delay_ms(1000);
  goto LOOP;
  
  return 0;
}
