/*
 * seven_segments.c
 *
 * Created: 10/9/2021 1:40:40 PM
 *  Author: Abdulrhman Nasser
 */ 

#include "DIO.h"
#define cc
void seven_seg_vinit(unsigned char port)
{
	DIO_set_port_direction(port,0xFF);
}
void seven_seg_write (unsigned char port, unsigned char val )
{
	#if defined cc
	unsigned char arr[]={ 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x47,0x7f,0x6f};
	#elif defined ca
	unsigned char arr[]={ ~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x47,~0x7f,~0x6f};
	#endif
	DIO_write_port(port,arr[val]);
}