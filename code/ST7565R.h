#define BLACK 1
#define WHITE 0

#define LCDWIDTH 128
#define LCDHEIGHT 32

#define FONT_OFFSET     32
#define FONT_WIDTH      5

void ST7565R_WriteCommand(unsigned char cmd);
void ST7565R_WriteData(unsigned char cmd);
void ST7565R_DisplayOn(unsigned char on);
void ST7565R_DisplayStartLine(unsigned char lineAddress);
void ST7565R_PageAddress(unsigned char pageAddress);
void ST7565R_ColumnAddress(unsigned char columnAddress);
void ST7565R_AdcReversed(unsigned char reversed);
void ST7565R_DisplayReversed(unsigned char reversed);
void ST7565R_DisplayAllPointsOn(unsigned char on);
void ST7565R_LcdBiasOneFifth(unsigned char oneFifth);
void ST7565R_ReadModifyWrite();
void ST7565R_End();
void ST7565R_Reset();
void ST7565R_CommonOutputReversed(unsigned char reversed);
void ST7565R_PowerController(unsigned char modes);
void ST7565R_ResistorRatio(unsigned char ratio);
void ST7565R_ElectronicVolumnMode(unsigned char value);
void ST7565R_Sleep(unsigned char sleep);
void ST7565R_BoosterRatio(unsigned char ratio);
void ST7565R_Nop();
void ST7565R_Init();
void ST7565R_Clear();
unsigned char ST7565R_32x24_Digit(unsigned char x, unsigned char col);
unsigned char ST7565R_32x24_Number(unsigned int x, unsigned char col);
void ST7565R_WriteChar(char ch);
void ST7565R_WriteInverseChar(char ch);
void ST7565R_WriteString(char * string);
void ST7565R_WriteInverseString(char * string);



