#include "delay.h"

static volatile uint32_t s_ticks = 0;

/**
 * @brief SysTick interrupt handler
 *
 * Increment the ms counter
 *
 */
void SysTick_Handler(void)
{
    s_ticks++;
}

/**
 * @brief Init SysTick
 *
 * Set SysTick to generate an interrupt every 1 ms
 *
 * @param cpu_hz CPU clock frequency in Hz
 */
void delay_init(uint32_t cpu_hz)
{
    STK->CTRL = 0;                        // arrête le timer pendant la config
    STK->LOAD = (cpu_hz / 1000u) - 1u;    // 1 ms par tick
    STK->VAL = 0;                         // n'importe quelle écriture remet à 0
    STK->CTRL = STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE;
}

/**
 * @brief Get time since init
 *
 * Return the number of ms since delay_init was called
 *
 * @return Time in ms
 */
uint32_t delay_millis(void)
{
    return s_ticks;
}

/**
 * @brief Delay in ms
 *
 * Blocking delay using the SysTick counter
 *
 * @param ms Delay in ms
 */
void delay_ms(uint32_t ms)
{
    uint32_t start = s_ticks;
    while ((s_ticks - start) < ms) {
        __asm volatile ("nop");           
    }
}