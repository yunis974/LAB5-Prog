#include "I2C.h"
#include "gpio.h"
#include "RCC.h"





/**
 * @brief Generate a stop condition
 *
 * Set the STOP bit and wait until the hardware clears it,
 * return if it times out
 *
 */
static void I2C_Stop(void)
{
    uint32_t timeout = I2C_TIMEOUT;
    I2C1->CR1 |= I2C_CR1_STOP;
    while (I2C1->CR1 & I2C_CR1_STOP)
    {
        if(timeout == 0)
        {
            return;
        }
        timeout--;
    }

}


/**
 * @brief Generate a start condition
 *
 * Set the START bit and wait for the SB flag,
 * send a stop if it times out
 *
 * @return true if the start was generated, false on timeout
 */
static bool I2C_Start(void)
{
    uint32_t timeout = I2C_TIMEOUT;
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB))
    {
        if (timeout == 0)
        {
            I2C_Stop();
            return false;
        }
        timeout--;
    }
    return true;
}



/**
 * @brief Check for acknowledge failure
 *
 * If the AF flag is set (slave sent a NACK), clear it and send a stop
 *
 * @return true if no AF, false if the slave sent a NACK
 */
static bool I2C_CheckAF(void)
{
    if(I2C1->SR1 & I2C_SR1_AF)
    {
        I2C1->SR1 = ~I2C_SR1_AF;
        I2C_Stop();
        return false;
    }
    return true;
}

/**
 * @brief Wait for a flag in SR1
 *
 * Wait until the desired flag is set, stop if the slave
 * sends a NACK or if it times out
 *
 * @param flag Desired SR1 flag (ex: I2C_SR1_ADDR)
 * @return true if the flag was set, false on NACK or timeout
 */
static bool I2C_WaitFlag(uint32_t flag)
{
    uint32_t timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & flag))
    {
        if (!I2C_CheckAF())
        {
            return false;
        }

        if (timeout == 0)
        {
            I2C_Stop();
            return false;
        }
        timeout--;
    }

    return true;
}




/**
 * @brief Init I2C1
 *
 * Enable I2C1 clock, set PB6 (SCL) and PB7 (SDA) in AF open drain,
 * configure the clock and enable the peripheral
 *
 */
void I2C_Init(void)
{
    RCC->APB1ENR |= (1 << I2C1_EN);
    GPIO_InitPin(GPIOB, 6, GPIO_MODE_AF_OD);
    GPIO_InitPin(GPIOB, 7, GPIO_MODE_AF_OD);

    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    I2C1->CR2 = 8;
    I2C1->CCR = 40;
    I2C1->TRISE = 9;
    I2C1->CR1 |= I2C_CR1_PE;

}


/**
 * @brief Write in a register
 *
 * Write one byte in the desired register of an I2C device
 *
 * @param address 7 bit address of the device
 * @param reg Register to write in
 * @param data Value to write
 * @return true if the write worked, false on error
 */
bool I2C_WriteRegister(uint8_t address, uint8_t reg, uint8_t data)
{

    if (!I2C_Start())
    {
        return false;
    }

    I2C1->DR = (address << 1);   

    if (!I2C_WaitFlag(I2C_SR1_ADDR))
    {
        return false;
    }



    (void)I2C1->SR1;
    (void)I2C1->SR2;

    I2C1->DR = reg;

    if (!I2C_WaitFlag(I2C_SR1_TXE))
    {
        return false;
    }


    I2C1->DR = data;

    if (!I2C_WaitFlag(I2C_SR1_BTF))
    {
        return false;
    }


    I2C_Stop();

    return true;

}

/**
 * @brief Read a register
 *
 * Read one byte from the desired register of an I2C device
 *
 * @param address 7 bit address of the device
 * @param reg Register to read
 * @param data Pointer where the read value is stored
 * @return true if the read worked, false on error
 */
bool I2C_ReadRegister(uint8_t address, uint8_t reg, uint8_t *data)
{
    if(!I2C_Start())
    {
        return false;
    }

    I2C1->DR = (address << 1);

    if (!I2C_WaitFlag(I2C_SR1_ADDR))
    {
        return false;
    }



    (void)I2C1->SR1;
    (void)I2C1->SR2;

    I2C1->DR = reg;

    if (!I2C_WaitFlag(I2C_SR1_BTF))
    {
        return false;
    }



    if(!I2C_Start())
    {
        return false;
    }

    I2C1->DR = (address << 1) | (1 << 0);

    if (!I2C_WaitFlag(I2C_SR1_ADDR))
    {
        return false;
    }



    I2C1->CR1 &= ~I2C_CR1_ACK;

    (void)I2C1->SR1;
    (void)I2C1->SR2;

    I2C_Stop();

    if (!I2C_WaitFlag(I2C_SR1_RXNE))
    {
        return false;
    }

    *data = I2C1->DR;
    return true;

}