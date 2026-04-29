#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "stm32f103xb.h"

void uart1_rxtx_init (void);
void uart1_tx_init (void);
char uart1_read (void);


#endif /* UART_H */

