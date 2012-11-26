#include <msp430.h>

#include "ST7565R.h"
#include "utils.h"
#include "spi.h"

/**
 * A0 (Select registers. 0: instruction,  1: data register) at P3.1
 */
#define A0      BIT1

extern unsigned char* numbers[10];
extern unsigned char font5x7[480];

                                     
void ST7565R_WriteData(unsigned char data) //Data Output Serial Interface
{
  P3OUT |= A0; // set A0 high to select data registers
  spi_csl();
  spi_xfer_byte(data);
  spi_csh();
}

void ST7565R_WriteCommand(unsigned char comm) //Command Output Serial Interface
{
  P3OUT &= ~A0; // set A0 low to select instruction registers
  spi_csl();
  spi_xfer_byte(comm);
  spi_csh();
}

void ST7565R_DisplayOn(unsigned char on)
{
  // The display can only be on or off, so we mask the last bit.
  ST7565R_WriteCommand(0xAE | (on & 0x01));
}

void ST7565R_DisplayStartLine(unsigned char lineAddress)
{
  // The line address can be from 0 to 63, so we mask the bottom 6 bits.
  ST7565R_WriteCommand(0x40 | (lineAddress & 0x3F));
}

void ST7565R_PageAddress(unsigned char pageAddress)
{
  // The page address from be from 0 tp 7, so we mask the bottom 4 bits.
  ST7565R_WriteCommand(0xB0 | (pageAddress & 0x0F));
}

void ST7565R_ColumnAddress(unsigned char columnAddress)
{
  // Send the column address in two commands, splitting the address
  // up into a hi and lo nibble.
  ST7565R_WriteCommand(0x10 | ((columnAddress >> 4) & 0x0F));
  ST7565R_WriteCommand(0x00 | (columnAddress & 0x0F));
}

void ST7565R_AdcReversed(unsigned char reversed)
{
  // The ADC allows normal and reverse directions.
  ST7565R_WriteCommand(0xA0 | (reversed & 0x01));
}

void ST7565R_DisplayReversed(unsigned char reversed)
{
  // The display allows the bits to be reversed, light on dark.
  ST7565R_WriteCommand(0xA6 | (reversed & 0x01));
}

void ST7565R_DisplayAllPointsOn(unsigned char on)
{
  // The display allows turing all points on without affecting
  // the display buffer. Setting back to normal displays the
  // buffer's contents again.
  ST7565R_WriteCommand(0xA4 | (on & 0x01));
}

void ST7565R_LcdBiasOneFifth(unsigned char oneFifth)
{
  // The default bias is 1/9, but can be changed to 1/5.
  ST7565R_WriteCommand(0xA2 | (oneFifth & 0x01));
}

void ST7565R_ReadModifyWrite()
{
  // Call this command to put the display in read-modify-write mode.
  ST7565R_WriteCommand(0xE0);
}

void ST7565R_End()
{
  // Call this command AFTER a call to ReadModifyWrite to take the
  // display out of read-modify-write mode.
  ST7565R_WriteCommand(0xEE);
}

void ST7565R_Reset()
{
  // Reinitializes the display.
  ST7565R_WriteCommand(0xE2);
}

void ST7565R_CommonOutputReversed(unsigned char reversed)
{
  // Display of lines are normally from COM0 to COM31. Reversing
  // this set the display crom COM31 to COM0, based on 1/33 duty
  // cycle for this display.
  ST7565R_WriteCommand(0xC0 | ((reversed & 0x01) << 3));
}

void ST7565R_PowerController(unsigned char modes)
{
  // Sets the power supply circuit functions,
  ST7565R_WriteCommand(0x28 | (modes & 0x07));
}

void ST7565R_ResistorRatio(unsigned char ratio)
{
  // Sets the volage regulator internal resistance resistor ratio.
  // Used for display contrast, values are from 0 to 7, small to high.
  ST7565R_WriteCommand(0x10 | (ratio & 0x07));
}

void ST7565R_ElectronicVolumnMode(unsigned char value)
{
  // This sets the liquid crystal drive voltage and helps control
  // the contrast of the display. The value range is from 0 to 63,
  // with 0 being the smallest voltage, and 63 the largest.
  ST7565R_WriteCommand(0x81);
  ST7565R_WriteCommand(value & 0x3F);
}

void ST7565R_Sleep(unsigned char sleep)
{
  // Puts the display into low current consumption mode. In this
  // case zero is sleep, and one is normal operation, so we toggle
  // the sleep value sent in. That way sleep=1, puts the display
  // to sleep, and sleep=0, wakes it back up.
  ST7565R_WriteCommand(0xAC | ((!sleep) & 0x01));
  ST7565R_WriteCommand(0x00);
}

void ST7565R_BoosterRatio(unsigned char ratio)
{
  // This sets the ratio for the booster cicuit. Possible values
  // are: 0=2x,3x,4x, 1=5x, 3=6x, 2 is not defined.
  ST7565R_WriteCommand(0xE8);
  ST7565R_WriteCommand(ratio & 0x03);
}

void ST7565R_Nop()
{
  ST7565R_WriteCommand(0xE3);
}


// LCD Init routine.
void ST7565R_Init()
{
  spi_init();

  // set pin directions
  P3DIR  |= A0;

  // toggle RST low to reset; CS low so it'll listen to us
  spi_csl();

  // LCD bias select
  ST7565R_LcdBiasOneFifth(0);
  // ADC select
  ST7565R_AdcReversed(0);
  // SHL select
  ST7565R_CommonOutputReversed(1);
  // Initial display line
  ST7565R_DisplayStartLine(0x00);

  // turn on voltage converter (VC=1, VR=0, VF=0)
  ST7565R_PowerController(0x4);
  // wait for 50% rising
  delay_ms(50);

  // turn on voltage regulator (VC=1, VR=1, VF=0)
  ST7565R_PowerController(0x6);
  // wait >=50ms
  delay_ms(50);

  // turn on voltage follower (VC=1, VR=1, VF=1)
  ST7565R_PowerController(0x7);
  // wait
  delay_ms(10);

  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  ST7565R_ResistorRatio(0);

  ST7565R_DisplayOn(1);
  ST7565R_DisplayAllPointsOn(0);
  ST7565R_ElectronicVolumnMode(0);
}


void ST7565R_Clear()
{
  unsigned int col;
  unsigned int page;

  ST7565R_DisplayOn(0);

  // Initialize DRAM
  for(page=0;page<4;page++)
  {
    ST7565R_PageAddress(page);
    for(col=0;col<131;col++)
    {
      ST7565R_ColumnAddress(col);
      ST7565R_WriteData(0);
    }
  }

  ST7565R_DisplayOn(1);
}

void ST7565R_WriteChar(char ch)
{
  unsigned char fontCol;
  for(fontCol=0; fontCol<FONT_WIDTH; fontCol++)
    ST7565R_WriteData(font5x7[((ch-FONT_OFFSET) * FONT_WIDTH) + fontCol]);

  ST7565R_WriteData(0);
}

void ST7565R_WriteInverseChar(char ch)
{
  unsigned char fontCol;
  for(fontCol=0; fontCol<FONT_WIDTH; fontCol++)
    ST7565R_WriteData(~font5x7[((ch-FONT_OFFSET) * FONT_WIDTH) + fontCol]);

  ST7565R_WriteData(0xFF);
}

void ST7565R_WriteString(char * string)
{
  while(*string)
  {
    ST7565R_WriteChar(*string++);
  }
}

void ST7565R_WriteInverseString(char * string)
{
  while(*string)
  {
    ST7565R_WriteInverseChar(*string++);
  }
}

void SetPixel(unsigned char x, unsigned char y)
{
  // x is column
  unsigned int column = x;

  // y / 8 is page
  unsigned int page = y / 8;

  ST7565R_ColumnAddress(column);
  ST7565R_PageAddress(page);

  y = 1 << (y % 8);
  ST7565R_WriteData(y);
}

unsigned char ST7565R_32x24_Digit(unsigned char x, unsigned char col)
{
  unsigned int i, page;

  for(page = 0; page < 4; page++)
  {
    ST7565R_PageAddress(page);   
    ST7565R_ColumnAddress(col); 
    for(i = 3; i < 22; i++)
      ST7565R_WriteData(*(numbers[x] + (page*24) + i));  
  }

  return col + 19;
}

unsigned char ST7565R_32x24_Number(unsigned int x, unsigned char col)
{
  unsigned char d5 = (x / 10000) % 10; // 6 digits is more than unsigned int can hold
  unsigned char d4 = (x / 1000) % 10;
  unsigned char d3 = (x / 100) % 10;
  unsigned char d2 = (x / 10) % 10;
  unsigned char d1 = x % 10;
  
  if((d5) > 0) col = ST7565R_32x24_Digit(d5,col);
  if((d5 | d4) > 0) col = ST7565R_32x24_Digit(d4,col);
  if((d5 | d4 | d3) > 0) col = ST7565R_32x24_Digit(d3,col);
  if((d5 | d4 | d3 | d2) > 0) col = ST7565R_32x24_Digit(d2,col);
  col = ST7565R_32x24_Digit(d1,col);
  
  return col;
}

