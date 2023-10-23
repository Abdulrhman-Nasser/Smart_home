/*
 * CPU.c
 *
 * Created: 10/6/2023 7:53:28 PM
 *  Author: Abdulrhman Nasser
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "Timers.h"
#include "SPI.h"
#include "CPU_Config.h"
#include "USART.h"
char data_room=0,hours=0,seconds=0,minutes=0,guest_pass1=0,guest_pass2=0,guest_pass3=0,guest_pass4=0,guest_flag=0;
void Recieve_Control()
{
	 DIO_write(control_int_pin,HIGH);
	 _delay_ms(INT_signal_time);
	 DIO_write(control_int_pin,LOW);
	 data_room=SPI_MasterTransmitchar('D',control_ss_pin);
	 DIO_write(control_int_pin,HIGH);
	 _delay_ms(INT_signal_time);
	 DIO_write(control_int_pin,LOW);
	 guest_flag=SPI_MasterTransmitchar('G',control_ss_pin);
	 DIO_write(control_int_pin,HIGH);
	 _delay_ms(INT_signal_time);
	 DIO_write(control_int_pin,LOW);
	 hours=SPI_MasterTransmitchar('H',control_ss_pin);
	 DIO_write(control_int_pin,HIGH);
	 _delay_ms(INT_signal_time);
	 DIO_write(control_int_pin,LOW);
	 minutes=SPI_MasterTransmitchar('M',control_ss_pin);
	 DIO_write(control_int_pin,HIGH);
	 _delay_ms(INT_signal_time);
	 DIO_write(control_int_pin,LOW);
	 seconds=SPI_MasterTransmitchar('S',control_ss_pin);
	 DIO_write(control_ss_pin,1);//deselect slave
}
void Send_Room()
{
	 DIO_write(room_int_pin,HIGH);
	 _delay_ms(INT_signal_time);
	 DIO_write(room_int_pin,LOW);
	 SPI_MasterTransmitchar(data_room,room_ss_pin);
	 DIO_write(room_int_pin,HIGH);
	 _delay_ms(INT_signal_time);
	 DIO_write(room_int_pin,LOW);
	 SPI_MasterTransmitchar(data_room,room_ss_pin);
	 DIO_write(room_ss_pin,1);//deselect slave
}
void Send_display()
{
	//DIO_write(display_int_pin,HIGH);
	//_delay_ms(INT_signal_time);
	//DIO_write(display_int_pin,LOW);
	//SPI_MasterTransmitchar(data_room,display_ss_pin);
	DIO_write(display_int_pin,HIGH);
	_delay_ms(INT_signal_time);
	DIO_write(display_int_pin,LOW);
	UART_vSendData(data_room);
	SPI_MasterTransmitchar(hours,display_ss_pin);
	DIO_write(display_int_pin,HIGH);
	_delay_ms(INT_signal_time);
	DIO_write(display_int_pin,LOW);
	UART_vSendData(data_room);
	SPI_MasterTransmitchar(minutes,display_ss_pin);
	DIO_write(display_int_pin,HIGH);
	_delay_ms(INT_signal_time);
	DIO_write(display_int_pin,LOW);
	UART_vSendData(data_room);
	SPI_MasterTransmitchar(seconds,display_ss_pin);
	DIO_write(display_ss_pin,1);//deselect slave
}
void Send_Garage()
{
	DIO_write(garage_int_pin,HIGH);
	_delay_ms(INT_signal_time);
	DIO_write(garage_int_pin,LOW);
	SPI_MasterTransmitchar(guest_flag,garage_ss_pin);
	DIO_write(garage_ss_pin,1);//deselect slave
}
int main(void)
{
	SPI_MasterInit();
	UART_vInit(9600);
	DIO_vsetPINDir(room_int_pin,OUTPUT);
	DIO_vsetPINDir(room_ss_pin,OUTPUT);
	DIO_vsetPINDir(control_int_pin,OUTPUT);
	DIO_vsetPINDir(control_ss_pin,OUTPUT);
	DIO_vsetPINDir(display_ss_pin,OUTPUT);
	DIO_vsetPINDir(display_int_pin,OUTPUT);
	DIO_vsetPINDir(garage_ss_pin,OUTPUT);
	DIO_vsetPINDir(garage_int_pin,OUTPUT);
    while(1)
    {
		Recieve_Control();
	   _delay_ms(SPI_delay);
		 Send_Room();
		_delay_ms(SPI_delay);
		Send_display();
		_delay_ms(SPI_delay);
		Send_Garage();
		_delay_ms(SPI_delay);
    }
}