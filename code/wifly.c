#include "wifly.h"
#include "uart.h"
#include "utils.h"

extern char RX_BUF[];
extern char MSG[];
extern unsigned int msg_received;

void wifly_rx(char c);

void wifly_init(char* ssid, char* password, unsigned long baud)
{
  uart_init(baud, wifly_rx);
}

void wifly_rx(char c) {

  static unsigned int i = 0;

  switch(c)
  {
    case -1:
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
