/*
 * Slave.c
 *
 * Created: 10/3/2023 9:53:55 AM
 *  Author: Abdulrhman Nasser
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "std_macros.h"
#include "SPI.h"
#include "LED.h"
#include "Room_config.h"
#include "std_macros.h"
#include "ADC.h"
char temp=18,data_room=0;
void VInit_external_interrupt()
{
	sei();
	SET_BIT(GICR,INT0);
	CLR_BIT(MCUCR,ISC00);
	SET_BIT(MCUCR,ISC01);
}
int main(void)
{	
	SPI_SlaveInit();
	VInit_external_interrupt();
	adc_vInit();
	LED_vInit(room1);
	LED_vInit(room2);
	LED_vInit(room3);
	LED_vInit(AC);
	while(1)
	{
		temp=(adc_u16read()*0.25);
		if(READ_BIT(data_room,led1_bit))
			LED_vTurnOn(room1);
		else 
			LED_vTurnOff(room1);
		if(READ_BIT(data_room,led2_bit))
			LED_vTurnOn(room2);
		else
			LED_vTurnOff(room2);
		if(READ_BIT(data_room,led3_bit))
			LED_vTurnOn(room3);
		else
			LED_vTurnOff(room3);
		if(READ_BIT(data_room,ac_status_bit))
		{
			if(temp>((data_room>>ac_temp_bit)+18))
			{
				LED_vTurnOn(AC);
			}
			else if(temp<((data_room>>ac_temp_bit)+18))
			{
				LED_vTurnOff(AC);
			}
		}
		else 
			LED_vTurnOff(AC);
		
	}
}
ISR(INT0_vect)
{
	
	data_room=SPI_SlaveReceivechar(0b00001111);
}