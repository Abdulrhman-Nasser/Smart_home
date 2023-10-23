/*
 * ADC.c
 *
 * Created: 1/18/2022 8:45:36 PM
 *  Author: Abdulrhman Nasser
 */ 
#include <avr/io.h>
#include "std_macros.h"

void adc_vInit (void)
{
	SET_BIT(ADMUX,REFS0); //Sets Reference voltage according to AVcc pin
	SET_BIT(ADMUX,REFS1);
	SET_BIT(ADCSRA,ADEN); //Enables ADC
	SET_BIT(ADCSRA,ADPS2);
	SET_BIT(ADCSRA,ADPS1); //Sets the clock to the MC clock(8Mhz) /64
	
}
unsigned short adc_u16read (void)
{
	SET_BIT(ADCSRA,ADSC);
	while(READ_BIT(ADCSRA,ADIF)==0);
	SET_BIT(ADCSRA,ADIF); //clears ADIF
	/*
	while(READ_BIT(ADCSRA,ADSC)==1);
	*/
	unsigned short read_val = ADCL;
	read_val|=(ADCH<<8);
	return read_val;
	
}	