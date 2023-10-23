/*
 * LCD.h
 *
 * Created: 10/23/2021 4:25:21 PM
 *  Author: Abdulrhman Nasser
 */ 


#ifndef LCD_H_
#define LCD_H_

#define data_port 'D'
#define control_port 'B'
#define four_bits_mode 

#define CLEAR_SCREEN 0x01
#define RETURN_HOME 0x02
#define SHIFT_CURSORL 0x04
#define SHIFT_DISPLAYR 0x05
#define ENTRY_MODE 0x06
#define SHIFT_DISPLAYL 0x07
#define DISPLAYOFF_CURSOROFF 0x08
#define DISPLAYOFF_CURSORON 0x0A
#define DISPLAYON_CURSOROFF 0x0C
#define DISPLAYON_CURSORON 0x0E
#define DISPLAYON_CURSORBLINK 0x0F

#if defined four_bits_mode
#define EN 0
#define RS 3
#define RW 1
#define FOUR_BITS 0x28
#elif defined eight_bits_mode
#define EN 0
#define RS 2
#define RW 1
#define EIGHT_BITS 0x38
#endif

void LCD_vInit();
void LCD_vSend_cmd(char cmd);
void LCD_Enable();
void LCD_vSend_char(char data);
void LCD_vSend_string(char *data);
void LCD_ClearScreen();
void LCD_movecursor(char row, char coloumn);
#endif /* LCD_H_ */