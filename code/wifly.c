#include "wifly.h"
#include "uart.h"
#include "utils.h"

extern char RX_BUF[];
extern char MSG[];
extern int msg_received;

void wifly_rx(char c);

void wifly_init(char* ssid, char* password, unsigned long baud)
{
  uart_init(9600, wifly_rx);
}

void wifly_rx(char c) {

  static unsigned int i = 0;

  switch(c)
  {
    case '\n':
      MSG[i] = 0;
      while(i)
      {
        i--;
        MSG[i] = RX_BUF[i];
        RX_BUF[i] = 0;
      }
      msg_received = 1;
      break;
    default:
      if(i >= RX_BUF_SIZE - 1)
        i = RX_BUF_SIZE - 2; // protect against overflow
      RX_BUF[i++] = c;
      break;
  }
}
