#ifndef PCA9685_h
#define PCA9685_h

#include <stdint.h>
#include <stdbool.h>

#define PCA9685_SERVO_MIN 105   // ~1 ms → 0°
#define PCA9685_SERVO_MAX 610   // ~2 ms → 180°
#define PCA9685_CH_MAX 15
#define PCA9685_PWM_MAX 4095
#define PCA9685_ADDR 0x40
#define PCA9685_MODE1 0x00
#define PCA9685_PRESCALE 0xFE
#define PCA9685_LED0_ON_L 0x06
#define PCA9685_RESTART (1 << 7)
#define PCA9685_EXTCLK (1 << 6)
#define PCA9685_AI (1 << 5)
#define PCA9685_SLEEP (1 << 4)
#define PCA9685_ALLCAL (1 << 0)

bool PCA9685_Init(void);
bool PCA9685_SetPWM(uint8_t canal,uint16_t start,uint16_t end);
bool PCA9685_SetAngle(uint8_t canal,uint8_t angle);




#endif