/*
 * main.c
 */

#include <msp430.h>
#include <signal.h>
#include <stdlib.h>

#include "ST7565R.h"
#include "utils.h"
#include "lm35.h"
#include "wifly.h"

// TIMER variables
volatile unsigned int timer = 0;

// UART variables
volatile unsigned int msg_received = 0;
char RX_BUF[RX_BUF_SIZE];
char MSG[RX_BUF_SIZE];

// HVAC control variables
int heat = 0;
int fan = 1;
int ac = 0;

// temperature variables
int cur_temp = 0;
int set_temp = 72;
int temps[100];
unsigned int temp_i = 0;
int offset = 0;

// encoder variables
unsigned char A_last = 1;
unsigned int off = 0;

int av_temp(int temp)
{
  temps[temp_i++] = temp;
  if(temp_i >= 100)
    temp_i = 0;

  int sum = 0;
  for(unsigned int i = 0; i < 100; i++)
      sum += temps[i];
  sum /= 100;

  return sum + offset;
}

void update_screen()
{
  ST7565R_32x24_Number(cur_temp,(128/2) - (19));

  ST7565R_PageAddress(0);
  ST7565R_ColumnAddress(128 - 18);
  ST7565R_WriteString("SET");

  char set_tempStr[] = "72F";
  itoa(set_temp,set_tempStr,10);
  set_tempStr[2] = 'F';
  ST7565R_PageAddress(1);
  ST7565R_ColumnAddress(128 - 18);
  ST7565R_WriteString(set_tempStr);

  ST7565R_PageAddress(1);
  ST7565R_ColumnAddress(0);
  if(heat) 
    ST7565R_WriteString("H ON ");
  else
    ST7565R_WriteString("H OFF");

  ST7565R_PageAddress(3);
  ST7565R_ColumnAddress(0);
  if(fan)
    ST7565R_WriteString("F ON ");
  else
    ST7565R_WriteString("F OFF");

  ST7565R_PageAddress(2);
  ST7565R_ColumnAddress(0);
  if(ac)
    ST7565R_WriteString("C ON ");
  else
    ST7565R_WriteString("C OFF");

  ST7565R_PageAddress(0);
  ST7565R_ColumnAddress(128/2+19);
  ST7565R_WriteData(0x00);
  ST7565R_WriteData(0x00);
  ST7565R_WriteData(0xFE);
  ST7565R_WriteData(0xFE);
  ST7565R_WriteData(0xFE);
  ST7565R_WriteData(0xCE);
  ST7565R_WriteData(0xCE);
  ST7565R_WriteData(0xCE);
  ST7565R_WriteData(0x0E);
  ST7565R_WriteData(0x0E);

  ST7565R_PageAddress(1);
  ST7565R_ColumnAddress(128/2+19);
  ST7565R_WriteData(0x00);
  ST7565R_WriteData(0x00);
  ST7565R_WriteData(0xFF);
  ST7565R_WriteData(0xFF);
  ST7565R_WriteData(0xFF);
  ST7565R_WriteData(0x01);
  ST7565R_WriteData(0x01);
  ST7565R_WriteData(0x01);
  ST7565R_WriteData(0x00);
  ST7565R_WriteData(0x00);
}

void read_encoder()
{
  if( (P2IN & BIT2) == BIT2 )
  {
    __asm("dint");
    off = ~off;
    __asm("eint");
    delay_ms(1000);
    return;
  }

  unsigned char A = ((P3IN & BIT2) >> 2) & 0x01; // enconder pin A is P3.2
  
  if(A == A_last)
    return;

  // software debounce
  for(unsigned int i = 0; i < 500; i++)
    if( (((P3IN & BIT2) >> 2) & 0x01) != A )
      return;

  if (!A_last && A) // if low to high transition
  {
    unsigned char B = ((P3IN & BIT3) >> 3) & 0x01; // enconder pin B is P3.3
    if (!B)
      set_temp--;
    else
      set_temp++;
  }
  A_last = A;
}

interrupt(TIMER0_A0_VECTOR) TIMERA_ISR(void)
{
  __asm("dint");

  if(!off)
  {
    fan = 1;
    P3OUT |= BIT4; // turn on fan
    if(set_temp > cur_temp)
    {
      heat = 1; P2OUT |=  BIT4;
      ac = 0;   P2OUT &= ~BIT3;
    }
    else if (set_temp < cur_temp)
    {
      heat = 0; P2OUT &= ~BIT4;
      ac = 1;   P2OUT |=  BIT3;
    }
    else
    {
      heat = ac = 0; 
      P2OUT &= ~BIT4;
      P2OUT &= ~BIT3;
    }
  }
  else
  {
    heat = ac = fan = 0; 
    P3OUT |= BIT4; // turn off fan
    P2OUT &= ~BIT4;
    P2OUT &= ~BIT3;
  }

  timer = 1;
  __asm("eint");
}

void init()
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;

  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TACTL = TASSEL_2 + MC_1 + ID_3;           // SMCLK/8, upmode  
  CCR0 =  10000;                            // 12.5 Hz 

  P3SEL = 0x00; // select io function for all ports
  P3DIR = 0x13; // P3.2 & P3.3 as input
  P3REN = 0x0C; // enable PULLUP/PULLDOWN
  P3OUT = 0x0C; // select PULLUP
  
  P2SEL = 0x00; // select io function for all ports
  P2DIR = BIT3 | BIT4; // enable output for HEAT and AC
  P2REN = BIT2; // enable PULLUP/PULLDOWN
  P2OUT = 0x00; // select PULLDOWN

  wifly_init("TylerN", "DZwV4FGb", 9600);
  ST7565R_Init();
  
  for(unsigned int i = 0; i < 100; i++)
    temps[i] = lm35_tempF(3.6, 0);

  ST7565R_Clear();

  __asm("eint");
}

int main(void)
{
  init();

  LOOP:

  while(!msg_received)
  {
    if(timer) // if timer expired run timer code
    {
        cur_temp = av_temp(lm35_tempF(3.6, 0));
        update_screen();
        __asm("dint"); timer = 0;  __asm("eint");
    }
    
    read_encoder(); // poll encoder
  }
  
  __asm("dint");

  // if a message has been received then change system state  
  set_temp = MSG[0];
  unsigned char clear = MSG[1];
  unsigned char vol = MSG[2];
  offset = MSG[3];

  ST7565R_DisplayOn(1);
  ST7565R_DisplayAllPointsOn(0);
  ST7565R_ElectronicVolumnMode(vol);

  if(clear) // if clear
    ST7565R_Clear();

  msg_received = 0;
  __asm("eint");

  goto LOOP;

  return 0;
}
