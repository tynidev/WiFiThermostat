#include <msp430.h>
#include "uart.h"
#include <signal.h>

/**
 * Callback handler for receive
 */
void (*uart_rx_isr_ptr)(char c);

void uart_init(unsigned long BAUD, void (*isr_ptr)(char c))
{
  uart_rx_isr_ptr = isr_ptr;

  P1SEL |= RXD + TXD ;                     // P1.3 = RXD, P1.2=TXD
  P1SEL2 |= RXD + TXD ;                    // P1.3 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK

  // Set the baudrate dividers and modulation
  unsigned int N_div = SYS_CLK / BAUD;
  float N_div_f = (float)SYS_CLK / BAUD;

  UCA0BR0 = (N_div & 0x00FF);
  UCA0BR1 = ((N_div & 0xFF00) >> 8);
  UCA0MCTL = (unsigned char)( (N_div_f - (unsigned int)N_div_f) * 8.0f ) << 1;

  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void uart_set_rx_isr_ptr(void (*isr_ptr)(char c))
{
  uart_rx_isr_ptr = isr_ptr;
}

char uart_getc()
{
  while (!(IFG2&UCA0RXIFG));  // USCI_A0 RX buffer ready?
  return UCA0RXBUF;
}

void uart_putc(char c)
{
  while (!(IFG2&UCA0TXIFG));  // USCI_A0 TX buffer ready?
  UCA0TXBUF = c;
}

void uart_puts(const char *str)
{
  while(*str) {
    uart_putc(*str++);
  }
}

// Echo back RXed character, confirm TX buffer is ready first
interrupt(USCIAB0RX_VECTOR) USCI0RX_ISR(void)
{
  if(uart_rx_isr_ptr != 0L) {
    (uart_rx_isr_ptr)(UCA0RXBUF);
  }
}

