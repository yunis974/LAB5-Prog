#include "gpio.h"
#include <stdint.h>
#include "lcd.h"
#include "I2C.h"
#include "adc.h"
#include "PCA9685.h"
#include "delay.h"

#define ABS(X) ((X) < 0 ? -(X) : (X))
#define ANGLE_THRESHOLD 5



int main(void)
{
    //init
    delay_init(8000000); //8Mhz
    LCD_Init();
    LCD_Clear();
    I2C_Init();
    ADC1_Init();
    PCA9685_Init();

    //variables
    uint8_t data = 0;
    char buf[12];
    uint16_t adc;
    uint32_t angle;
    uint8_t previous_angle = 0;
    uint32_t previous_time;

    if(!(I2C_ReadRegister(0x40, 0x00, &data)))
    {
        LCD_SendString("I2C Error");
        return 1;
    }

    
    LCD_SetCursor(0,0);
    LCD_SendString("ANGLE=");

    previous_time = delay_millis();

    while(1)
    {
        adc = ADC1_Read();
        angle = (adc * 180) / 4095;
        ADC_ValueToString(angle,buf);
        LCD_SetCursor(0,7);

        uint32_t now = delay_millis();
        if (now - previous_time > 100)
        {
            LCD_SendString(buf);
            previous_time = delay_millis();
        }

        if (ABS((uint32_t)angle - (uint32_t)previous_angle) > ANGLE_THRESHOLD)
        {
            PCA9685_SetAngle(0, angle);
            previous_angle = angle;
        }

        
    }

    return 0;
}
