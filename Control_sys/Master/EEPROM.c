/*
 * EEPROM.c
 *
 * Created: 10/21/2021 1:03:48 PM
 *  Author: Abdulrhman Nasser
 */ 


#include <avr/io.h>
#include "std_macros.h"
void EEPROM_write(unsigned short address,unsigned char data )
{
	EEAR=address;
	EEDR=data;
	SET_BIT(EECR,EEMWE);
	SET_BIT(EECR,EEWE);
	while(READ_BIT(EECR,EEWE)==1);
	
}
unsigned char EEPROM_read (unsigned short adress)
{
	EEAR=adress;
	SET_BIT(EECR,EERE);
	return EEDR;
}