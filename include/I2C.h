#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>

#define I2C1_EN 21
#define I2C1 ((I2C_t*)0x40005400)
#define I2C_CR1_SWRST (1 << 15)
#define I2C_CR1_PE (1 << 0)
#define I2C_CR1_ACK (1 << 10)
#define I2C_CR1_START (1 << 8)
#define I2C_CR1_STOP (1 << 9)
#define I2C_SR1_SB (1 << 0)
#define I2C_SR1_ADDR (1 << 1)
#define I2C_SR1_TXE (1 << 7)
#define I2C_SR1_BTF (1 << 2)
#define I2C_SR1_RXNE (1<< 6)
#define I2C_SR1_AF (1 << 10)
#define I2C_TIMEOUT 100000


typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
} I2C_t;

void I2C_Init(void);
bool I2C_WriteRegister(uint8_t address, uint8_t reg, uint8_t data);
bool I2C_ReadRegister(uint8_t address, uint8_t reg, uint8_t *data);



#endif