#include "gpio.h"
#include "RCC.h"


/**
 * @brief Init gpio pin
 * 
 * Takes the index of the desired port, the desired pin, 
 * and the desired mode
 * 
 * @param port_index Index of the port you want to use (A:0, B:1, C:2)
 * @param pin Desired pin
 * @param mode Desired gpio mode
 * 
 */
void GPIO_InitPin(char port_index, uint8_t pin, uint8_t mode)
{
    if (port_index > 2 || pin > 15)
    {
        return;
    }
    //activate selected port clock
    RCC->APB2ENR |= (1 << (port_index + 0x02));
    //select register CRL or CRH
    if (pin > 7)
    {
        uint8_t shift = (pin % 8) * 4;
        PORT(port_index)->CRH &= ~(0xF << shift);
        PORT(port_index)->CRH |= (mode << shift);
    } 
    else
    {
        uint8_t shift = pin * 4;
        PORT(port_index)->CRL &= ~(0xF << shift);
        PORT(port_index)->CRL |= (mode << shift);
    }
}



/**
 * @brief Write on a pin
 * 
 * Write on desired pin
 * @param port_index Index of the port you want to use (A:0, B:1, C:2)
 * @param pin Desired pin
 * @param value Desired value
 */
void GPIO_WritePin(char port_index, uint8_t pin, bool value)
{
    if (port_index > 2 || pin > 15)
    {
        return;
    }

    if (value)
    {
        PORT(port_index)->BSRR = (1UL << pin);
    }
    else
    {
        PORT(port_index)->BSRR = (1UL << (pin + 16));
    }
}


/**
 * @brief Read pin
 * 
 * Return value of the desired pin
 * 
 * @param port_index Index of the port you want to use (A:0, B:1, C:2)
 * @param pin Desired pin
 */
bool GPIO_ReadPin(char port_index, uint8_t pin)
{
    return ((PORT(port_index)->IDR & (1 << pin)));
}
