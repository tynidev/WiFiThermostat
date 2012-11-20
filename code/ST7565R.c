/*
 * Project name:
 Sitronix ST7565R Dot Matix LCD Controller/Driver Library (SPI)
 * Copyright:
 (c) Sophtware.com, Inc., 2010.
 * Revision History:
 20101218:
 - initial release;
 * Description:
 The ST7565R-C12832A1Z is a small (32mmx8mm) 128x32 COG display with an SPI
 interface. Only 5 pins are need to control the display; SDA, SCL, RST,
 A0, and CS. The /RST pin can be eliminated with the use of a pullup
 resistor, but would give you less control over the display. This display
 uses an ST7565R controller and can be had for less than $12 USD.

 Display Datasheet: http://www.newhavendisplay.com/specs/ST7565R-C12832A1Z-FSW-FBW-3V3.pdf
 Controller DataSheet: http://www.newhavendisplay.com/app_notes/ST7565R.pdf
 Manufacture Website: http://www.newhavendisplay.com/
 * Test configuration:
 MCU:             PIC18F45K20
 Dev.Board:       EasyLV18F v6
 Oscillator:      HS, 8.0000 MHz
 Ext. Modules:    ST7565R-C12832A1Z with ST7565R-PCB12832A1Z breakout board.
 SW:              mikroC PRO for PIC
 http://www.mikroe.com/eng/products/view/7/mikroc-pro-for-pic/
 * NOTES:
 - Breakout board connections:
 HDR FUNC  PORT NOTES
 --- ----- ---- -----
 1   VSS   GND
 2   VDD   3v3
 3   SDA   RC5  Data out
 4   SCL   RC3  Clock
 5   A0    RB0  Command/Data toggle
 6   RST   RB1  Hardware reset
 7   CS1B  RA5  Chip select
 8   LED+  3v3
*/


#include <msp430.h>

#include "ST7565R.h"
#include "utils.h"
#include "spi.h"

/**
 * A0 (Select registers. 0: instruction,  1: data register) at P3.1
 */
#define A0      BIT1

const unsigned char font5x7[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00,// (space)
  0x00, 0x00, 0x5F, 0x00, 0x00,// !
  0x00, 0x07, 0x00, 0x07, 0x00,// "
  0x14, 0x7F, 0x14, 0x7F, 0x14,// #
  0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
  0x23, 0x13, 0x08, 0x64, 0x62,// %
  0x36, 0x49, 0x55, 0x22, 0x50,// &
  0x00, 0x05, 0x03, 0x00, 0x00,// '
  0x00, 0x1C, 0x22, 0x41, 0x00,// (
  0x00, 0x41, 0x22, 0x1C, 0x00,// )
  0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
  0x08, 0x08, 0x3E, 0x08, 0x08,// +
  0x00, 0x50, 0x30, 0x00, 0x00,// ,
  0x08, 0x08, 0x08, 0x08, 0x08,// -
  0x00, 0x30, 0x30, 0x00, 0x00,// .
  0x20, 0x10, 0x08, 0x04, 0x02,// /
  0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
  0x00, 0x42, 0x7F, 0x40, 0x00,// 1
  0x42, 0x61, 0x51, 0x49, 0x46,// 2
  0x21, 0x41, 0x45, 0x4B, 0x31,// 3
  0x18, 0x14, 0x12, 0x7F, 0x10,// 4
  0x27, 0x45, 0x45, 0x45, 0x39,// 5
  0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
  0x01, 0x71, 0x09, 0x05, 0x03,// 7
  0x36, 0x49, 0x49, 0x49, 0x36,// 8
  0x06, 0x49, 0x49, 0x29, 0x1E,// 9
  0x00, 0x36, 0x36, 0x00, 0x00,// :
  0x00, 0x56, 0x36, 0x00, 0x00,// ;
  0x00, 0x08, 0x14, 0x22, 0x41,// <
  0x14, 0x14, 0x14, 0x14, 0x14,// =
  0x41, 0x22, 0x14, 0x08, 0x00,// >
  0x02, 0x01, 0x51, 0x09, 0x06,// ?
  0x32, 0x49, 0x79, 0x41, 0x3E,// @
  0x7E, 0x11, 0x11, 0x11, 0x7E,// A
  0x7F, 0x49, 0x49, 0x49, 0x36,// B
  0x3E, 0x41, 0x41, 0x41, 0x22,// C
  0x7F, 0x41, 0x41, 0x22, 0x1C,// D
  0x7F, 0x49, 0x49, 0x49, 0x41,// E
  0x7F, 0x09, 0x09, 0x01, 0x01,// F
  0x3E, 0x41, 0x41, 0x51, 0x32,// G
  0x7F, 0x08, 0x08, 0x08, 0x7F,// H
  0x00, 0x41, 0x7F, 0x41, 0x00,// I
  0x20, 0x40, 0x41, 0x3F, 0x01,// J
  0x7F, 0x08, 0x14, 0x22, 0x41,// K
  0x7F, 0x40, 0x40, 0x40, 0x40,// L
  0x7F, 0x02, 0x04, 0x02, 0x7F,// M
  0x7F, 0x04, 0x08, 0x10, 0x7F,// N
  0x3E, 0x41, 0x41, 0x41, 0x3E,// O
  0x7F, 0x09, 0x09, 0x09, 0x06,// P
  0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
  0x7F, 0x09, 0x19, 0x29, 0x46,// R
  0x46, 0x49, 0x49, 0x49, 0x31,// S
  0x01, 0x01, 0x7F, 0x01, 0x01,// T
  0x3F, 0x40, 0x40, 0x40, 0x3F,// U
  0x1F, 0x20, 0x40, 0x20, 0x1F,// V
  0x7F, 0x20, 0x18, 0x20, 0x7F,// W
  0x63, 0x14, 0x08, 0x14, 0x63,// X
  0x03, 0x04, 0x78, 0x04, 0x03,// Y
  0x61, 0x51, 0x49, 0x45, 0x43,// Z
  0x00, 0x00, 0x7F, 0x41, 0x41,// [
  0x02, 0x04, 0x08, 0x10, 0x20,// "\"
  0x41, 0x41, 0x7F, 0x00, 0x00,// ]
  0x04, 0x02, 0x01, 0x02, 0x04,// ^
  0x40, 0x40, 0x40, 0x40, 0x40,// _
  0x00, 0x01, 0x02, 0x04, 0x00,// `
  0x20, 0x54, 0x54, 0x54, 0x78,// a
  0x7F, 0x48, 0x44, 0x44, 0x38,// b
  0x38, 0x44, 0x44, 0x44, 0x20,// c
  0x38, 0x44, 0x44, 0x48, 0x7F,// d
  0x38, 0x54, 0x54, 0x54, 0x18,// e
  0x08, 0x7E, 0x09, 0x01, 0x02,// f
  0x08, 0x14, 0x54, 0x54, 0x3C,// g
  0x7F, 0x08, 0x04, 0x04, 0x78,// h
  0x00, 0x44, 0x7D, 0x40, 0x00,// i
  0x20, 0x40, 0x44, 0x3D, 0x00,// j
  0x00, 0x7F, 0x10, 0x28, 0x44,// k
  0x00, 0x41, 0x7F, 0x40, 0x00,// l
  0x7C, 0x04, 0x18, 0x04, 0x78,// m
  0x7C, 0x08, 0x04, 0x04, 0x78,// n
  0x38, 0x44, 0x44, 0x44, 0x38,// o
  0x7C, 0x14, 0x14, 0x14, 0x08,// p
  0x08, 0x14, 0x14, 0x18, 0x7C,// q
  0x7C, 0x08, 0x04, 0x04, 0x08,// r
  0x48, 0x54, 0x54, 0x54, 0x20,// s
  0x04, 0x3F, 0x44, 0x40, 0x20,// t
  0x3C, 0x40, 0x40, 0x20, 0x7C,// u
  0x1C, 0x20, 0x40, 0x20, 0x1C,// v
  0x3C, 0x40, 0x30, 0x40, 0x3C,// w
  0x44, 0x28, 0x10, 0x28, 0x44,// x
  0x0C, 0x50, 0x50, 0x50, 0x3C,// y
  0x44, 0x64, 0x54, 0x4C, 0x44,// z
  0x00, 0x08, 0x36, 0x41, 0x00,// {
  0x00, 0x00, 0x7F, 0x00, 0x00,// |
  0x00, 0x41, 0x36, 0x08, 0x00,// }
  0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
  0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
};

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
  for(page=0;page<8;page++)
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

void DisplayBox()
{
  unsigned col;
  ST7565R_DisplayOn(0);
  ST7565R_DisplayStartLine(0x00);

  // Top
  ST7565R_PageAddress(0);
  ST7565R_ColumnAddress(0);
  ST7565R_WriteData(0xFF);
  for(col=0;col<126;col++)  //128 columns wide
    ST7565R_WriteData(0x01);
  ST7565R_WriteData(0xFF);

  // Sides
  ST7565R_PageAddress(1);
  ST7565R_ColumnAddress(0);
  ST7565R_WriteData(0xFF);
  ST7565R_ColumnAddress(127);
  ST7565R_WriteData(0xFF);

  ST7565R_PageAddress(2);
  ST7565R_ColumnAddress(0);
  ST7565R_WriteData(0xFF);
  ST7565R_ColumnAddress(127);
  ST7565R_WriteData(0xFF);

  // Bottom
  ST7565R_PageAddress(3);
  ST7565R_ColumnAddress(0);
  ST7565R_WriteData(0xFF);
  for(col=0;col<126;col++)  //128 columns wide
    ST7565R_WriteData(0x80);
  ST7565R_WriteData(0xFF);

  ST7565R_DisplayOn(1);
}
