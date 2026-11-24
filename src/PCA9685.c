#include "PCA9685.h"
#include "I2C.h"
#include "delay.h"

/**
 * @brief Init PCA9685
 *
 * Put the PCA9685 in sleep, set the prescaler for 50 Hz,
 * then wake it up with auto increment
 *
 * @return true
 */
bool PCA9685_Init(void)
{
    I2C_WriteRegister(PCA9685_ADDR, PCA9685_MODE1, PCA9685_SLEEP); 
    I2C_WriteRegister(PCA9685_ADDR, PCA9685_PRESCALE, 121); 
    I2C_WriteRegister(PCA9685_ADDR, PCA9685_MODE1, PCA9685_AI); 
    delay_ms(1);

    return true;
}

/**
 * @brief Set PWM on a channel
 *
 * Write the ON and OFF counts of the desired channel
 *
 * @param canal Desired channel (0 to 15)
 * @param start Count where the output goes high (0 to 4095)
 * @param end Count where the output goes low (0 to 4095)
 * @return false if a parameter is out of range, true otherwise
 */
bool PCA9685_SetPWM(uint8_t canal,uint16_t start,uint16_t end)
{
    if (canal > PCA9685_CH_MAX || start > PCA9685_PWM_MAX || end > PCA9685_PWM_MAX)
    {
        return false;
    }

    uint8_t reg = PCA9685_LED0_ON_L + (4 * canal);
    
    I2C_WriteRegister(PCA9685_ADDR, reg, start & 0xFF);
    I2C_WriteRegister(PCA9685_ADDR, reg + 1, start >> 8);

    I2C_WriteRegister(PCA9685_ADDR, reg + 2, end & 0xFF);
    I2C_WriteRegister(PCA9685_ADDR, reg + 3, end >> 8);

    return true;

}   

/**
 * @brief Set servo angle
 *
 * Convert the angle to a pulse between PCA9685_SERVO_MIN
 * and PCA9685_SERVO_MAX and send it on the desired channel
 *
 * @param canal Desired channel (0 to 15)
 * @param angle Desired angle (0 to 180, clamped)
 * @return false if the channel is out of range, true otherwise
 */
bool PCA9685_SetAngle(uint8_t canal,uint8_t angle)
{
    if (angle > 180)
    {
        angle = 180;
    }

    uint16_t ticks = PCA9685_SERVO_MIN + ((uint32_t)angle * (PCA9685_SERVO_MAX - PCA9685_SERVO_MIN)) / 180;

    return PCA9685_SetPWM(canal, 0, ticks);
}