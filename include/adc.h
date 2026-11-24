#ifndef adc_h
#define adc_h

#include <stdint.h>

#define ADC1 ((ADC_t *)0x40012400)
#define RCC_APB2ENR_ADC1EN (1 << 9)
#define ADC_SR_EOC         (1 << 1)
#define ADC_SQR3_SQ1       (0x1 << 0)
#define ADC_CR2_ADON       (1 << 0)
#define ADC_CR2_CONT       (1 << 1)
#define ADC_CR2_CAL        (1 << 2)
#define ADC_CR2_RSTCAL     (1 << 3)
#define ADC_CR2_EXTSEL     (0x7 << 17)
#define ADC_CR2_EXTRIG     (1 << 20)
#define ADC_CR2_SWSTART    (1 << 22)

typedef struct
{
    volatile uint32_t SR;   
    volatile uint32_t CR1;  
    volatile uint32_t CR2;  
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;  
    volatile uint32_t LTR;  
    volatile uint32_t SQR1; 
    volatile uint32_t SQR2; 
    volatile uint32_t SQR3; 
    volatile uint32_t JSQR; 
    volatile uint32_t JDR1; 
    volatile uint32_t JDR2; 
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
} ADC_t;


void ADC1_Init(void);
uint16_t ADC1_Read(void);

void ADC_ValueToString(uint16_t value, char *buffer);



#endif