#include "gpio.h"
#include <stdint.h>
#include "lcd.h"
#include "I2C.h"
#include "adc.h"
#include "PCA9685.h"
#include "delay.h"


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
    if(!(I2C_ReadRegister(0x40, 0x00, &data)))
    {
        LCD_SendString("I2C Err");
        return 1;
    }

    LCD_SetCursor(0,0);
    LCD_SendString("ANGLE=");
    while(1)
    {
        adc = ADC1_Read();
        angle = (adc * 180) / 4095;
        ADC_ValueToString(angle,buf);
        LCD_SetCursor(0,7);
        LCD_SendString(buf);
        PCA9685_SetAngle(0, angle);

        delay_ms(500);
    }

    return 0;
}
