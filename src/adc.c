#include "adc.h"
#include "gpio.h"
#include "RCC.h"


/**
 * @brief Init ADC1
 *
 * Enable ADC1 clock, set PA0 in analog, configure software
 * trigger in single conversion mode, power on and calibrate
 *
 */
void ADC1_Init(void) {
    //adc clock enable
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    GPIO_InitPin(GPIOA, 0, GPIO_MODE_ANALOG);
    //ADC1->SQR3 |= ADC_SQR3_SQ1;

    ADC1->CR2 |= ADC_CR2_EXTSEL;
    ADC1->CR2 |= ADC_CR2_EXTRIG;
    ADC1->CR2 &= ~ADC_CR2_CONT;
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    ADC1->CR2 |= ADC_CR2_CAL;
}

/**
 * @brief Read ADC1
 *
 * Start a conversion and wait until it is done
 *
 * @return Converted value (0 to 4095)
 */
uint16_t ADC1_Read(void) {
    uint16_t adc_value;
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC)) {}
    adc_value = (uint16_t)(ADC1->DR & 0xFFFF);

    return adc_value;
}

/**
 * @brief Convert an angle to a string
 *
 * Write the value on 3 digits followed by the degree symbol
 *
 * @param value Value to convert (0 to 999)
 * @param buffer Output string, needs at least 5 chars
 */
void ADC_ValueToString(uint16_t value, char *buffer) {
    int working_value = value;
    for(int i = 2; i >= 0 ; i--) {
        buffer[i] = (working_value % 10) + '0';
        working_value /= 10;
    }
    buffer[3] = 0xDF;
    buffer[4] = '\0';
}