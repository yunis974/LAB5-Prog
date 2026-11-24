#include "lcd.h"
#include "delay.h"
#include "gpio.h"
//pin used by the lcd on our stm32 devboard
//PBX
#define RS_Pin 9      
#define RW_Pin 10
#define EN_Pin 11
#define D4_Pin 12
#define D5_Pin 13
#define D6_Pin 14
#define D7_Pin 15








/**
 * @brief Init LCD pins
 *
 * Enable GPIOB clock and set PB9 to PB15 in output
 *
 */
void LCD_GPIO_Init(void)
{
    for (int i = 0; i < 7; i++)
    {
        GPIO_InitPin(GPIOB, RS_Pin + i, GPIO_MODE_OUTPUT_PP);
    }

}


/**
 * @brief Write on a PORTB pin
 *
 * Write on desired pin of PORTB
 *
 * @param pin Desired pin
 * @param value Desired value
 */
void WriteToPin(uint8_t pin, bool value)
{   
    GPIO_WritePin(GPIOB, pin, value);
}


/**
 * @brief Send 4 bits to the LCD
 *
 * Put the 4 bits on D4 to D7 and pulse EN
 *
 * @param data Value to send (only the 4 low bits are used)
 */
void LCD_Send4Bits(uint8_t data)
{
    for (int i = 0; i < 4; i++)
    {
        WriteToPin(D4_Pin + i, (data >> i) & 0x01);
    }
    
    WriteToPin(EN_Pin, 1);
    delay_ms(1);
    WriteToPin(EN_Pin, 0);

}


/**
 * @brief Send a byte to the LCD
 *
 * Set RW and RS then send the byte in two parts of 4 bits
 *
 * @param RW Read (1) or write (0)
 * @param RS Data (1) or command (0)
 * @param data Byte to send
 */
void LCD_Command(bool RW, bool RS, uint8_t data)
{
    WriteToPin(RW_Pin, RW);
    WriteToPin(RS_Pin, RS);

    LCD_Send4Bits(data >> 4);
    LCD_Send4Bits(data & 0x0F);


}


/**
 * @brief Init LCD
 *
 * Init pins, then set the LCD in 4 bit mode,
 * 2 lines, display on, cursor off and clear it
 * (delay_init must be called before)
 *
 */
void LCD_Init(void)
{
    LCD_GPIO_Init();
    delay_ms(50);

    LCD_Command(0, 0, 0x33);
    delay_ms(2);
    LCD_Command(0, 0, 0x32);

    LCD_Command(0,0,0x28);
    LCD_Command(0,0,0x0C);
    LCD_Command(0,0,0x06);
    LCD_Command(0,0,0x01);

    delay_ms(2);
}

/**
 * @brief Write a char
 *
 * Write one char at the cursor position
 *
 * @param character Char to write
 */
void LCD_WriteChar(char character)
{
    WriteToPin(RS_Pin, 1);
    WriteToPin(RW_Pin, 0);

    LCD_Send4Bits(character >> 4);
    LCD_Send4Bits(character & 0x0F);
}

/**
 * @brief Write a string
 *
 * Write a string starting at the cursor position
 *
 * @param string String to write (must end with '\0')
 */
void LCD_SendString(char *string)
{
    while (*string)
    {
        LCD_WriteChar(*string++);
    }
}

/**
 * @brief Clear LCD
 *
 * Clear the display and put the cursor back at 0,0
 *
 */
void LCD_Clear(void)
{
    LCD_Command(0, 0, 0x01);
    delay_ms(2);
}

/**
 * @brief Set cursor position
 *
 * Move the cursor to the desired row and column
 *
 * @param row Desired row (0 to 3)
 * @param col Desired column (0 to 19)
 */
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t rows[4] = {0x00, 0x40, 0x14, 0x54};
    if (row > 3) return;
    if (col > 19) return;

    uint8_t address = rows[row] + col;

    uint8_t com = (1u << 7) | (address & 0x7F);
    LCD_Command(0, 0 , com);
    
    delay_ms(2);
    
}

