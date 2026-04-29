#include "uart.h"

#define GPIOBEN     (1<<3)
#define AFIOEN      (1<<0)
#define UART1EN     (1<<14)

#define CR1_TE      (1<<3)
#define CR1_RE      (1<<2)
#define CR1_UE      (1<<13)
#define SR_TXE      (1<<7)
#define SR_RXNE     (1<<5)

#define SYSCLK_FREQ 8000000
#define APB2_CLK     SYSCLK_FREQ

#define UART1_BAUDRATE 115200

static void uart1_set_baudrate (USART_TypeDef *USARTx, uint32_t PeriphClock, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClock, uint32_t BaudRate);


void uart1_write (int ch);

int __io_putchar(int ch)
{
    uart1_write(ch);
    return ch;
}

void uart1_tx_init (void)
{
    /********************Configure UART1 TX pin**********************/

    //1. Enable clock access to GPIOB
    RCC->APB2ENR |= GPIOBEN;
    
    //2. Enable clock access to AFIO
    RCC->APB2ENR |= AFIOEN;

    //3. Set PB6 mode to alternate function output push-pull
    GPIOB->CRL &= ~(0xF << 24); // Clear the bits for PB6
    GPIOB->CRL |= (0xB << 24); // Set the bits for PB6 to alternate function output push-pull (0b1011)

    //4. Set PB6 alternate function type to UART1_REMAP=1
    AFIO->MAPR |= (1 << 2); // Set the bit for UART1_REMAP to 1`

    /********************Configure UART1 Module**********************/
    //1. Enable clock access to UART1
    RCC->APB2ENR |= UART1EN;

    //2. Configure baud rate
    uart1_set_baudrate(USART1, APB2_CLK, UART1_BAUDRATE);

    //3. Configure transfer direction (TX only)
    USART1->CR1 |= CR1_TE; // Enable transmitter only

    //4. Enable UART1 module
    USART1->CR1 |= CR1_UE; // Enable UART1
}

void uart1_rxtx_init (void)
{
    /********************Configure UART1 TX pin**********************/

    //1. Enable clock access to GPIOB
    RCC->APB2ENR |= GPIOBEN;
    
    //2. Enable clock access to AFIO
    RCC->APB2ENR |= AFIOEN;

    //3. Set PB6 mode to alternate function output push-pull
    GPIOB->CRL &= ~(0xF << 24); // Clear the bits for PB6
    GPIOB->CRL |= (0xB << 24); // Set the bits for PB6 to alternate function output push-pull (0b1011)

    //4. Set PB6 alternate function type to UART1_REMAP=1
    AFIO->MAPR |= (1 << 2); // Set the bit for UART1_REMAP to 1`

    //5. Set PB7 mode to alternate function output push-pull
    GPIOB->CRL &= ~(0xF << 28);
    GPIOB->CRL |= (0x4 << 28); // Input floating (0b0100)

    //6. Set PB7 alternate function type to UART1_REMAP=1
    AFIO->MAPR |= (1 << 2); // Set the bit for UART1_REMAP to 1

    /********************Configure UART1 Module**********************/
    //1. Enable clock access to UART1
    RCC->APB2ENR |= UART1EN;

    //2. Configure baud rate
    uart1_set_baudrate(USART1, APB2_CLK, UART1_BAUDRATE);

    //3. Configure transfer direction (TX & RX)
    USART1->CR1 |= (CR1_TE | CR1_RE); // Enable both transmitter and receiver

    //4. Enable UART1 module
    USART1->CR1 |= CR1_UE; // Enable UART1
}

char uart1_read (void)
{
    /*Make sure the receive data register is not empty*/
    while(!(USART1->SR & SR_RXNE)){}
    /*Read data from receive data register*/
    return USART1->DR;
}

void uart1_write (int ch)
{
    /*Make sure the transmit data register is empty*/
    while(!(USART1->SR & SR_TXE)){}
    /*Write to transmit data register*/
    USART1->DR = (ch & 0xFF);


}

static void uart1_set_baudrate (USART_TypeDef *USARTx, uint32_t PeriphClock, uint32_t BaudRate)
{
    USARTx->BRR = compute_uart_bd(PeriphClock, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClock, uint32_t BaudRate)
{
    return ((PeriphClock + (BaudRate/2U)) / BaudRate);
}
