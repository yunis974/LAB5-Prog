#include "timer.h"
#include "RCC.h"

static void (*userCallback)(void) = 0;

/**
 * @brief Init TIM2
 *
 * Enable TIM2 clock, set prescaler and auto reload,
 * enable the update interrupt and start the timer
 *
 */
void timerInit(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_ENTIM2;
    TIM2->PSC = TIM2_PSC_VAL;
    TIM2->ARR = TIM2_ARR_VAL;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->SR  = ~TIM_SR_UIF;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC->ISER0 |= (1 << TIM2_IRQn);
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief Set timer callback
 *
 * Set the function called on each TIM2 interrupt
 *
 * @param f Function to call
 */
void setTimerCallback(void(*f)(void))
{
    userCallback = f;
}


/**
 * @brief TIM2 interrupt handler
 *
 * Clear the update flag and call the user callback if there is one
 *
 */
void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR = ~TIM_SR_UIF;
        if (userCallback) userCallback();    
    }
}