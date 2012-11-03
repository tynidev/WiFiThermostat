#ifndef __UART_H
#define __UART_H

/**
 * Receive Data (RXD) at P3.5
 */
#define RXD		BIT5

/**
 * Transmit Data (TXD) at P3.4
 */
#define TXD		BIT4

/**
 * Initialize soft UART
 */
void uart_init(unsigned long BAUD, void (*isr_ptr)(char c));

/**
 * Set pointer for ISR to call when data was received.
 *
 * @param[in]	*isr_ptr	pointer to ISR
 */
void uart_set_rx_isr_ptr(void (*isr_ptr)(char c));

char uart_getc();

/**
 * Write one chracter to the UART blocking.
 *
 * @param[in]	*c	the character to write
 */
void uart_putc(char c);

/**
 * Write string to the UART blocking.
 *
 * @param[in]	*str	the 0 terminated string to write
 */
void uart_puts(const char *str);

#endif
