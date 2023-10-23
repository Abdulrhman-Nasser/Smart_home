/*
 * Servo_driver.c
 *
 * Created: 4/24/2023 8:37:43 AM
 *  Author: Abdulrhman Nasser
 */ 
#include <avr/io.h>
#include "Timers.h"
void servo_write (double pos)
{
	double val= (1+(pos/180));
	Timer1A_fastPWM_wave(val);
}