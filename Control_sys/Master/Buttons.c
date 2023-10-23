/*
 * Buttons.c
 *
 * Created: 10/3/2021 3:34:33 PM
 *  Author: Abdulrhman Nasser
 */


#include "DIO.h"

void Button_vInit(unsigned char portname, unsigned char pinnumber)
{
	DIO_vsetPINDir(portname,pinnumber,0);
}
unsigned char Button_u8read(unsigned char portname, unsigned char pinnumber)
{
	unsigned char x=DIO_u8read(portname,pinnumber);
	return x;
}
void Button_InternalPullUp(unsigned char portname, unsigned char pinnumber, unsigned char enable)
{
    DIO_vconnectpullup(portname,pinnumber,enable);
}
