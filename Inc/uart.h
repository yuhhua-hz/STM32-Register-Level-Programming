#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uart_conf(void);
void uart_send_string(const char *str);
char uart_receive_char(void);
uint8_t uart_data_available(void);

#endif // UART_H_
