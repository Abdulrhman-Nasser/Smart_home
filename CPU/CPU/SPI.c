/*
 * SPI.c
 *
 * Created: 8/3/2023 12:10:46 PM
 *  Author: Abdulrhman Nasser
 */ 
#include <avr/io.h>
#include "DIO.h"
#include "std_macros.h"
#include "CPU_Config.h"
#define F_CPU 8000000
#include <avr/delay.h>
void SPI_MasterInit(void)
{
	/*Set SS / MOSI / SCK  as output pins for master*/
	DIO_vsetPINDir(room_ss_pin,1);
	DIO_vsetPINDir(control_ss_pin,1);
	DIO_vsetPINDir(display_ss_pin,1);
	DIO_vsetPINDir('B',5,1);
	DIO_vsetPINDir('B',7,1);
	SET_BIT(SPCR,MSTR);//Selects master mode
	CLR_BIT(SPCR,SPR0);
	CLR_BIT(SPCR,SPR1);
	CLR_BIT(SPSR,SPI2X); //Set frequency of master to Focs/4
	SET_BIT(SPCR,SPE);//Enables SPI
	/*enable interrupt*/
	//SET_BIT(SPCR,SPIE);
	CLR_BIT(SPCR,CPOL);
	CLR_BIT(SPCR,CPHA); //Leading edge->sample (rising)   Trailling edge->Setup (falling)
	/*set SS to high*/
	DIO_write(room_ss_pin,1);
	DIO_write(control_ss_pin,1);
	
}
void SPI_SlaveInit(void)
{
	CLR_BIT(SPCR,MSTR);//Selects slave mode
	SET_BIT(SPCR,SPE);//Enables SPI
	/*Set MISO as output*/
	DIO_vsetPINDir('B',6,1);
}
unsigned char SPI_MasterTransmitchar(unsigned char Data,unsigned char slave_port,unsigned char slave_pin)
{
	/*Clear SS to send data to slave*/
	DIO_write(slave_port,slave_pin,0);
	/*put data on SPDR*/
	SPDR=Data;
	while(READ_BIT(SPSR,SPIF)==0);
	/*read the received data*/
	//DIO_write(slave_port,slave_pin,1); //Deselect slave
	return SPDR ;
}
unsigned char SPI_SlaveReceivechar(unsigned char Data)
{
	/*Put data on SPDR*/
	SPDR=Data;
	/*Wait until data is received in SPI register*/
	while(READ_BIT(SPSR,SPIF)==0);
	/*read the received data*/
	return SPDR ;
}

void SPI_MasterTransmitstring(unsigned char *ptr,unsigned char slave_port,unsigned char slave_pin)
{
	while((*ptr)!=0)
	{
		SPI_MasterTransmitchar(*ptr,slave_port,slave_pin);
		_delay_ms(300);
		ptr++;
	}
}