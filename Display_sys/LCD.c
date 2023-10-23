/*
 * LCD.c
 *
 * Created: 10/23/2021 4:22:59 PM
 *  Author: Abdulrhman Nasser
 */
#include "DIO.h"
#include "std_macros.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include "LCD.h"
void LCD_vInit()
{
	#if defined eight_bits_mode
	DIO_vsetPINDir(data_port,0,1);
	DIO_vsetPINDir(data_port,1,1);
	DIO_vsetPINDir(data_port,2,1);
	DIO_vsetPINDir(data_port,3,1);
	DIO_vsetPINDir(data_port,4,1);
	DIO_vsetPINDir(data_port,5,1);
	DIO_vsetPINDir(data_port,6,1);
	DIO_vsetPINDir(data_port,7,1);
	DIO_vsetPINDir(control_port,RS,1);
	DIO_vsetPINDir(control_port,EN,1);
	DIO_vsetPINDir(control_port,RW,1);
	DIO_write(control_port,RW,0);
	LCD_vSend_cmd(RETURN_HOME);
	_delay_ms(10);
	LCD_vSend_cmd(EIGHT_BITS); //8 bit mode
	_delay_ms(1);
	LCD_vSend_cmd(DISPLAYON_CURSORON);
	_delay_ms(1);
	LCD_vSend_cmd(CLEAR_SCREEN);
	_delay_ms(10);
	LCD_vSend_cmd(ENTRY_MODE);
	_delay_ms(1);

	#elif defined four_bits_mode
	DIO_vsetPINDir(data_port,4,1);
	DIO_vsetPINDir(data_port,5,1);
	DIO_vsetPINDir(data_port,6,1);
	DIO_vsetPINDir(data_port,7,1);
	DIO_vsetPINDir(control_port,RS,1);
	DIO_vsetPINDir(control_port,EN,1);
	DIO_vsetPINDir(control_port,RW,1);
	DIO_write(control_port,RW,0);
	LCD_vSend_cmd(RETURN_HOME);
	_delay_ms(10);
	LCD_vSend_cmd(FOUR_BITS); //4 bit mode
	_delay_ms(1);
	LCD_vSend_cmd(DISPLAYON_CURSORON);
	_delay_ms(1);
	LCD_vSend_cmd(CLEAR_SCREEN);
	_delay_ms(10);
	LCD_vSend_cmd(ENTRY_MODE);
	_delay_ms(1);
	#endif
}
void LCD_vSend_cmd(char cmd)
{
    #if defined eight_bits_mode
	DIO_write_port(data_port,cmd);
	DIO_write(control_port,RS,0);
	LCD_Enable();
	#elif defined four_bits_mode
		DIO_write_high_nibble(data_port,cmd>>4);
		DIO_write(control_port,RS,0);
		LCD_Enable();
		DIO_write_high_nibble(data_port,cmd);
		DIO_write(control_port,RS,0);
		LCD_Enable();
	#endif
	_delay_ms(1);
}
void LCD_Enable()
{
	DIO_write(control_port,EN,1);
	_delay_ms(2);
	DIO_write(control_port,EN,0);
	_delay_ms(2);
}
void LCD_vSend_char(char data)
{
	#if defined eight_bits_mode
		DIO_write_port(data_port,data);
		DIO_write(control_port,RS,1);
		LCD_Enable();

	#elif defined four_bits_mode
		DIO_write_high_nibble(data_port,data>>4);
		DIO_write(control_port,RS,1);
		LCD_Enable();
		DIO_write_high_nibble(data_port,data);
		DIO_write(control_port,RS,1);
		LCD_Enable();
	#endif
	_delay_ms(1);
}
void LCD_vSend_string(char *data)
{
	while((*data)!='\0')
	{
		LCD_vSend_char(*data);
		data++;
	}
}
void LCD_ClearScreen()
{
	LCD_vSend_cmd(CLEAR_SCREEN);
	_delay_ms(10);
}
void LCD_movecursor(char row, char coloumn)
{
	char temp;
	if(row>2||row<1||coloumn>16||coloumn<1)
	{
		LCD_vSend_cmd(0x80);
	}
	else if(row==1)
	{
		temp=0x80+coloumn-1;

	}
	else if(row==2)
	{
		temp=0xC0+coloumn-1;

	}
	LCD_vSend_cmd(temp);
	_delay_ms(1);
}
