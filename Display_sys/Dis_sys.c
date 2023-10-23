/*
 * Dis_sys.c
 *
 * Created: 10/13/2023 3:04:11 PM
 *  Author: Abdulrhman Nasser
 */
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "std_macros.h"
#include "SPI.h"
#include "seven segments.h"
#include "LCD.h"
#include "Display_config.h"
#include "Timers.h"
#include "USART.h"
volatile unsigned short seconds=0;
volatile unsigned short minutes=0;
volatile unsigned short hours=0;
volatile unsigned char s='D';
volatile unsigned char data_room=0,temp=0,data_room1=0;
void VInit_external_interrupt()
{
	sei();
	SET_BIT(GICR,INT2);
	CLR_BIT(MCUCR,ISC00);
	SET_BIT(MCUCR,ISC01);
}
void dis_Vinit()
{
	VInit_external_interrupt();
	vInit_Timer2_NormalMode();
	seven_seg_vinit(seven_seg_port);
	SPI_SlaveInit();
	LCD_vInit();
	UART_vInit(9600);
	DIO_vsetPINDir(S1,1);
	DIO_vsetPINDir(S2,1);
	DIO_vsetPINDir(S3,1);
	DIO_vsetPINDir(S4,1);
	DIO_vsetPINDir(S5,1);
	DIO_vsetPINDir(S6,1);
}
int main(void)
{
	dis_Vinit();
	_delay_ms(180);


	minutes=0;
	seconds=0;
	hours=0;
		LCD_vSend_string("L1: OFF  L2: OFF");
		LCD_movecursor(2,1);
		LCD_vSend_string("L3: OFF  AC: OFF");
	while(1)
	{
		DIO_write(S1,1);
		DIO_write(S2,1);
		DIO_write(S3,1);
		DIO_write(S4,1);
		DIO_write(S5,1);
		DIO_write(S6,0);
		seven_seg_write(seven_seg_port,seconds%10);
		_delay_ms(5);
		
		
		DIO_write(S1,1);
		DIO_write(S2,1);
		DIO_write(S3,1);
		DIO_write(S4,1);
		DIO_write(S5,0);
		DIO_write(S6,1);
		seven_seg_write(seven_seg_port,seconds/10);
		_delay_ms(5);
		
		
		DIO_write(S1,1);
		DIO_write(S2,1);
		DIO_write(S3,1);
		DIO_write(S4,0);
		DIO_write(S5,1);
		DIO_write(S6,1);
		seven_seg_write(seven_seg_port,minutes%10);
		_delay_ms(5);
		
		
		DIO_write(S1,1);
		DIO_write(S2,1);
		DIO_write(S3,0);
		DIO_write(S4,1);
		DIO_write(S5,1);
		DIO_write(S6,1);
		seven_seg_write(seven_seg_port,minutes/10);
		_delay_ms(5);
		
		
		DIO_write(S1,1);
		DIO_write(S2,0);
		DIO_write(S3,1);
		DIO_write(S4,1);
		DIO_write(S5,1);
		DIO_write(S6,1);
		seven_seg_write(seven_seg_port,hours%10);
		_delay_ms(5);
		
		
		DIO_write(S1,0);
		DIO_write(S2,1);
		DIO_write(S3,1);
		DIO_write(S4,1);
		DIO_write(S5,1);
		DIO_write(S6,1);
		seven_seg_write(seven_seg_port,hours/10);
		_delay_ms(5);
		
		if (seconds >=60)
		{
			minutes++;
			seconds=0;
		}
		if (minutes >= 60)
		{
			hours++;
			minutes=0;
		}
		if (hours >=24)
		{
			hours=0;
		}
		if(data_room!=data_room1)
		{

			data_room1=data_room;
			if(READ_BIT(data_room1,led1_bit))
			{
				LCD_movecursor(1,6);
				LCD_vSend_string("N ");
			}
			else
			{
				LCD_movecursor(1,6);
				LCD_vSend_string("FF");	
			}
			if(READ_BIT(data_room1,led2_bit))
			{
				LCD_movecursor(1,15);
				LCD_vSend_string("N ");
			}
			else
			{
				LCD_movecursor(1,15);
				LCD_vSend_string("FF");
			}
			if(READ_BIT(data_room1,led3_bit))
			{
				LCD_movecursor(2,6);
				LCD_vSend_string("N ");
			}
			else
			{
				LCD_movecursor(2,6);
				LCD_vSend_string("FF");
			}
			if(READ_BIT(data_room1,ac_status_bit))
			{
				LCD_movecursor(2,15);
				LCD_vSend_string("N ");
			}
			else
			{
				LCD_movecursor(2,15);
				LCD_vSend_string("FF");
			}
		}
		
	}
}
ISR(TIMER2_OVF_vect)
{
	seconds++;

}
ISR(INT2_vect)
{
		//data_room=SPI_SlaveReceivechar('C');
		data_room=UART_u8ReceiveData();
		temp=SPI_SlaveReceivechar('A');
		//LCD_vSend_char(temp);
		if(READ_BIT(temp,7))
		{
			CLR_BIT(temp,7);
			if (seconds!=temp)
				{seconds=temp;}
		}
		if(READ_BIT(temp,6))
		{
			CLR_BIT(temp,6);
			if (minutes!=temp)
				{minutes=temp;}
		}

		if(READ_BIT(temp,5))
		{
			CLR_BIT(temp,5);
			if (hours!=temp)
				{hours=temp;}
		}


}