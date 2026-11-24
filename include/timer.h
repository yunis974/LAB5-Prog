#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>


#define TIM2 ((TIM_t *)0x40000000)
#define RCC_APB1ENR_ENTIM2 (1 << 0)   
#define TIM2_PSC_VAL 7999
#define TIM2_ARR_VAL 999
#define TIM2_IRQn 28

#define TIM_CR1_CEN  (1 << 0)
#define TIM_DIER_UIE (1 << 0)
#define TIM_SR_UIF   (1 << 0)
#define TIM_EGR_UG   (1 << 0)

#define NVIC ((NVIC_t *)0xE000E100)

typedef struct
{
    volatile uint32_t ISER0;
    volatile uint32_t ISER1;
    volatile uint32_t ISER2;
} NVIC_t;

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR; //only for tim1 and tim8
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR; //only for tim1 and tim8
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
}TIM_t;

void timerInit(void);
void setTimerCallback(void(*f)(void));

#endif