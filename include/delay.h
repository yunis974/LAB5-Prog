#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

#define STK ((STK_t *)0xE000E010)
#define STK_CTRL_ENABLE    (1 << 0)
#define STK_CTRL_TICKINT   (1 << 1)
#define STK_CTRL_CLKSOURCE (1 << 2)
#define STK_CTRL_COUNTFLAG (1 << 16)

typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} STK_t;

void     delay_init(uint32_t cpu_hz);
uint32_t delay_millis(void);
void     delay_ms(uint32_t ms);



#endif 