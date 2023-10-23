/*
 * Garage_system.c
 *
 * Created: 10/13/2023 10:00:03 PM
 *  Author: Abdulrhman Nasser
 */ 



#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "Timers.h"
#include "Servo_driver.h"
#include "LCD.h"
#include "SPI.h"
#include "std_macros.h"
char guest_flag=0;
void VInit_external_interrupt()
{
	DIO_vsetPINDir('D',2,0);
	sei();
	SET_BIT(GICR,INT0);
	CLR_BIT(MCUCR,ISC00);
	SET_BIT(MCUCR,ISC01);
}
void Garage_VInit()
{
	
	LCD_vInit();
	SPI_SlaveInit();
	VInit_external_interrupt();
}
int main(void)
{
	Garage_VInit();
	char flag=0;
	while(1)
	{
		if(!READ_BIT(guest_flag,0))
		{
			LCD_vSend_string("Set Guest pass");
			LCD_movecursor(2,1);
			LCD_vSend_string("From Control sys");
		}
		else 
		{
			
			
			if(READ_BIT(guest_flag,1))
			{
				servo_write(180);
				if(flag==1)
					{	LCD_ClearScreen();
						LCD_vSend_string("Garage opened");
						flag=0;}
			}
			else 
			{
				servo_write(0);
				if(flag==0)
					{	LCD_ClearScreen();
						LCD_vSend_string("Garage Closed");
						flag=1;}
			}
		}
	}
}
ISR(INT0_vect)
{
	guest_flag=SPI_SlaveReceivechar('F');
}