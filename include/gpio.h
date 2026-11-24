#ifndef gpio_h
#define gpio_h

#include <stdint.h>
#include <stdbool.h>

//pin modes (CNF[1:0] MODE[1:0]), outputs at 10 MHz
#define GPIO_MODE_ANALOG         0x0
#define GPIO_MODE_INPUT_FLOATING 0x4
#define GPIO_MODE_INPUT_PULL     0x8   // pull-up if ODR = 1, pull-down if ODR = 0
#define GPIO_MODE_OUTPUT_PP      0x1
#define GPIO_MODE_OUTPUT_OD      0x5
#define GPIO_MODE_AF_PP          0x9
#define GPIO_MODE_AF_OD          0xD

#define PORTA 0x40010800
#define PORT_OFFSET 0x400
#define PORT(i) ((GPIO_t *)((uint8_t*)PORTA + PORT_OFFSET * (i)))
#define GPIOA 0
#define GPIOB 1
#define GPIOC 2

typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_t;



void GPIO_InitPin(char port_index, uint8_t pin, uint8_t mode);
void GPIO_WritePin(char port_index, uint8_t pin, bool value); 
bool GPIO_ReadPin(char port_index, uint8_t pin);



#endif