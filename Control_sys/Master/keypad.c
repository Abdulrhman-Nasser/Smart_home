/*
 * keypad.c
 *
 * Created: 11/9/2021 8:26:14 PM
 *  Author: Abdulrhman Nasser
 */ 
#define port 'C'
#define NOTPRESSED 0xff
#include "Buttons.h"
#include "DIO.h"
void keypad_vInit()
{
	DIO_vsetPINDir(port,0,1);
	DIO_vsetPINDir(port,1,1);
	DIO_vsetPINDir(port,2,1);
	DIO_vsetPINDir(port,3,1);
	DIO_vsetPINDir(port,4,0);
	DIO_vsetPINDir(port,5,0);
	DIO_vsetPINDir(port,6,0);
	DIO_vsetPINDir(port,7,0);
	DIO_vconnectpullup(port,4,1);
	DIO_vconnectpullup(port,5,1);
	DIO_vconnectpullup(port,6,1);
	DIO_vconnectpullup(port,7,1);
	
}
char keypad_u8check_press()
{
	char arr[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'A','0','=','+'}};
	char row,coloumn,x;
	char returnval=NOTPRESSED;
	for(row=0;row<=3;row++)
	{
		DIO_write_low_nibble(port,0xff);
		DIO_write(port,row,0);
		for(coloumn=0;coloumn<=3;coloumn++)
		{
			x=Button_u8read(port,(coloumn+4));
			if(x==0)
			{
				returnval=arr[row][coloumn];
				break;
			}
		}
		if(x==0)
		{
			break;
		}
	}
	return returnval;
	
	
}