/*
 * Master.c
 *
 * Created: 10/3/2023 9:53:06 AM
 *  Author: Abdulrhman Nasser
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "SPI.h"
#include "keypad.h"
#include "Config.h"
#include "std_macros.h"
#include "USART.h"
#include "Timers.h"
volatile unsigned char data_room=0,hours=0,seconds=0,minutes=0,time_flag=0,guest_flag=0,counter=0;
/* ***************************************************************************   */
			/* Welcome the user when the system starts */
void welcome()
{
	LCD_vSend_string(Wlcm_msg1);
	LCD_movecursor(2,1);
	LCD_vSend_string(Wlcm_msg2);
	_delay_ms(wlcm_msg_time);
	LCD_ClearScreen();
	if(EEPROM_read(Conseq_trials_status)==Conseq_trials-1)
		sys_block();
}
void VInit_external_interrupt()
{
	sei();
	SET_BIT(GICR,INT0);
	CLR_BIT(MCUCR,ISC00);
	SET_BIT(MCUCR,ISC01);
}
/* Initializations */
void init()
{
	LCD_vInit();
	keypad_vInit();
	SPI_SlaveInit();
	VInit_external_interrupt();
	UART_vInit(9600);
	vInit_Timer0_CTC();
	#ifdef first_time
	EEPROM_write(sys_en_status,1);
	EEPROM_write(sys_hack_check1,90);
	EEPROM_write(sys_hack_check2,10);
	EEPROM_write(sys_hack_check3,20);
	#endif

}
/* ***************************************************************************   */
			/* Detecting if the user has already set a password or not, sends them either to signup or login  */
void check_first_time()
{
	if(EEPROM_read(blocked_status)==1)
		blocked();
	char status=both_pass_Set;
	if(EEPROM_read(admin_pass_status)==0xFF && EEPROM_read(guest_pass_status)==0xFF)
		status=No_pass_Set;
	else if(EEPROM_read(admin_pass_status)==0xFF)
		status=admin_pass_NSet;
	else if(EEPROM_read(guest_pass_status)==0xFF)
		status=guest_pass_NSet;
	switch (status){
		case No_pass_Set:
		set_both_pass();
		break;
		case admin_pass_NSet:
		set_admin_pass();
		break;
		case guest_pass_NSet:
		set_guest_pass();
		break;
		}
	
}
/* Signup of admin and guest  */
void set_both_pass()
{
	set_admin_pass();
	set_guest_pass();
}
void set_admin_pass()
{
	LCD_ClearScreen();
	LCD_vSend_string("Set admin pass: ");
	LCD_movecursor(2,1);
	for(char i=0;i<=3;i++)
	{
		char x;
		L:x=keypad_u8check_press();
		_delay_ms(200);
		while (x==NOTPRESSED)
		{
			x=keypad_u8check_press();
			_delay_ms(200);
		}
		if(x=='*' || x=='/' || x=='-' || x=='+' && i!=0)
			{
				LCD_movecursor(2,i);
				LCD_vSend_char(' ');
				LCD_movecursor(2,i);
				EEPROM_write(admin_pass_address+i-1,0xff);
				i--;
				goto L;
			}				
		EEPROM_write(admin_pass_address+i,x);
		LCD_vSend_char(x);
		_delay_ms(show_pass_time);
		LCD_movecursor(2,1+i);
		LCD_vSend_char(hidden_char);
	}
	EEPROM_write(admin_pass_status,1);
}
void set_guest_pass()
{
	LCD_ClearScreen();
	LCD_vSend_string("Set guest pass: ");
	LCD_movecursor(2,1);
	for(char i=0;i<=3;i++)
	{	char x;
		L: x=keypad_u8check_press();
		_delay_ms(200);
		while (x==NOTPRESSED)
		{
			x=keypad_u8check_press();
			_delay_ms(200);
		}
		if(x=='*' || x=='/' || x=='-' || x=='+' && i!=0)
		{
			LCD_movecursor(2,i);
			LCD_vSend_char(' ');
			LCD_movecursor(2,i);
			EEPROM_write(guest_pass_address+i-1,0xff);
			i--;
			goto L;
		}
		
			EEPROM_write(guest_pass_address+i,x);
			LCD_vSend_char(x);
			_delay_ms(show_pass_time);
			LCD_movecursor(2,1+i);
			LCD_vSend_char(hidden_char);
				
	}
	EEPROM_write(guest_pass_status,1);
	
}
/* ***************************************************************************   */
			/* Let the user choose the mode he wants to enter, either admin or guest  */
void choose_mode()
{
	guest_flag=0x01;
	LCD_ClearScreen();
	LCD_vSend_string(choose_mode_str1);
	LCD_movecursor(2,1);
	LCD_vSend_string(choose_mode_str2);
	char x=keypad_u8check_press();
	_delay_ms(200);
	while (x!=admin_mode && x!=guest_mode)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x==admin_mode)
		admin_login();
	else if(x==guest_mode)
		guest_login();
	
}
/* Login for admin  */
void admin_login()
{
	char flag,trials=Number_of_trials;
	L:flag=0;
	LCD_ClearScreen();
	LCD_vSend_string("Enter admin pass: ");
	LCD_movecursor(2,1);
	for(char i=0;i<=3;i++)
	{
		char x=keypad_u8check_press();
		_delay_ms(200);
		while (x==NOTPRESSED)
		{
			x=keypad_u8check_press();
			_delay_ms(200);
		}
		LCD_vSend_char(x);
		_delay_ms(show_pass_time);
		LCD_movecursor(2,1+i);
		LCD_vSend_char(hidden_char);
		if(x!=EEPROM_read(admin_pass_address+i))
		{
			flag++;
		}
	}
	if(!flag)
		{	
			admin();
		}
	else 
		{
			trials--;
			LCD_ClearScreen();
			LCD_vSend_string("Wrong pass");
			LCD_movecursor(2,1);
			LCD_vSend_string("Trials left: ");
			LCD_vSend_char(trials+48);
			_delay_ms(wlcm_msg_time);
			EEPROM_write(Conseq_trials_status,EEPROM_read(Conseq_trials_status)+1);
			if(EEPROM_read(Conseq_trials_status)==Conseq_trials-1)
				sys_block();
			
			if(trials==0)
			{
				blocked();
			}
			else 
				goto L;
		}
	
}
/* Login for guest  */
void guest_login()
{
	char flag,trials=Number_of_trials;
	L:flag=0;
	LCD_ClearScreen();
	LCD_vSend_string("Enter guest pass: ");
	LCD_movecursor(2,1);
	for(char i=0;i<=3;i++)
	{
		char x=keypad_u8check_press();
		_delay_ms(200);
		while (x==NOTPRESSED)
		{
			x=keypad_u8check_press();
			_delay_ms(200);
		}
		LCD_vSend_char(x);
		_delay_ms(show_pass_time);
		LCD_movecursor(2,1+i);
		LCD_vSend_char(hidden_char);
		if(x!=EEPROM_read(guest_pass_address+i))
		{
			flag++;
		}
	}
	if(!flag)
	{
		
		guest();
	}
	else
	{
		trials--;
		LCD_ClearScreen();
		LCD_vSend_string("Wrong pass");
		LCD_movecursor(2,1);
		LCD_vSend_string("Trials left: ");
		LCD_vSend_char(trials+48);
		_delay_ms(wlcm_msg_time);
		EEPROM_write(Conseq_trials_status,EEPROM_read(Conseq_trials_status)+1);
		if(EEPROM_read(Conseq_trials_status)==Conseq_trials-1)
			sys_block();
		if(trials==0)
		{
			
			blocked();
		}
		else
		goto L;
	}
	
}
void blocked()
{
	LCD_ClearScreen();
	EEPROM_write(blocked_status,1);
	LCD_vSend_string(blocked_msg1);
	LCD_movecursor(2,1);
	LCD_vSend_string(blocked_msg2);
	LCD_vSend_char((blocked_time/100)+48);
	LCD_vSend_char((blocked_time/10)%10+48);
	LCD_vSend_char((blocked_time%10)+48);
	LCD_vSend_string("sec");
	for(char i=0;i<blocked_time;i++)
		_delay_ms(1000);
	EEPROM_write(blocked_status,0);
	choose_mode();
}
sys_block()
{
	LCD_ClearScreen();
	LCD_vSend_string("System Blocked");
	while(1);
}
void admin()
{
	LCD_ClearScreen();
	LCD_vSend_string("1:Room  2:Garage");
	LCD_movecursor(2,1);
	LCD_vSend_string("3:Display");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A')
		choose_mode();
	else if(x=='1')
	{
		room_sys();
	}
	else if(x=='2')
	{
		garage_sys(2);
	}
	else if(x=='3')
	{
		display_sys();
	}
	else 
		goto L;
}
void room_sys()
{
	LCD_ClearScreen();
	LCD_vSend_string("1:LED1   2:LED2");
	LCD_movecursor(2,1);
	LCD_vSend_string("3:LED3   4:AC");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A')
		admin();
	else if(x=='1')
	{
		led1();
	}
	else if(x=='2')
	{
		led2();
	}
	else if(x=='3')
	{
		led3();
	}
	else if(x=='4')
	{
		ac();
	}
	else
	goto L;
}
void led1()
{
	LCD_ClearScreen();
	LCD_vSend_string("1:Turn On    LED");
	LCD_movecursor(2,1);
	LCD_vSend_string("2:Turn Off    1");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A')
		room_sys();
	else if(x=='1')
	{
		led1_turnOn();
	}
	else if(x=='2')
	{
		led1_turnOff();
	}
	else
	goto L;
}
void led1_turnOn()
{
	LCD_ClearScreen();
	//LCD_vSend_string("LED1 Turned on");
	SET_BIT(data_room,led1_bit);
	led1();
	
}
void led1_turnOff()
{
	LCD_ClearScreen();
	//LCD_vSend_string("LED1 Turned off");
	CLR_BIT(data_room,led1_bit);
	led1();
}
void led2()
{
	LCD_ClearScreen();
	LCD_vSend_string("1:Turn On    LED");
	LCD_movecursor(2,1);
	LCD_vSend_string("2:Turn Off    2");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A')
		room_sys();
	else if(x=='1')
	{
		led2_turnOn();
	}
	else if(x=='2')
	{
		led2_turnOff();
	}
	else
	goto L;
}
void led2_turnOn()
{
	LCD_ClearScreen();
	//LCD_vSend_string("LED2 Turned on");
	SET_BIT(data_room,led2_bit);
	led2();
}
void led2_turnOff()
{
	LCD_ClearScreen();
	//LCD_vSend_string("LED2 Turned off");
	CLR_BIT(data_room,led2_bit);
	led2();
}
void led3()
{
	LCD_ClearScreen();
	LCD_vSend_string("1:Turn On    LED");
	LCD_movecursor(2,1);
	LCD_vSend_string("2:Turn Off    3");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A')
		room_sys();
	else if(x=='1')
	{
		led3_turnOn();
	}
	else if(x=='2')
	{
		led3_turnOff();
	}
	else
	goto L;
}
void led3_turnOn()
{
	LCD_ClearScreen();
	//LCD_vSend_string("LED3 Turned on");
	SET_BIT(data_room,led3_bit);
	led3();
}
void led3_turnOff()
{
	LCD_ClearScreen();
	//LCD_vSend_string("LED3 Turned off");
	CLR_BIT(data_room,led3_bit);
	led3();
}
void ac()
{
	LCD_ClearScreen();
	LCD_vSend_string("1:Control       AC");
	LCD_movecursor(2,1);
	LCD_vSend_string("2:Temperature");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A')
		room_sys();
	else if(x=='1')
	{
		ac_control();
	}
	else if(x=='2')
	{
		ac_temp();
	}
	else
	goto L;
}
void ac_control()
{
	LCD_ClearScreen();
	LCD_vSend_string("1:Turn On     AC");
	LCD_movecursor(2,1);
	LCD_vSend_string("2:Turn Off");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A')
		ac();
	else if(x=='1')
	{
		ac_turnOn();
	}
	else if(x=='2')
	{
		ac_turnOff();
	}
	else
	goto L;
}
void ac_turnOn()
{
	LCD_ClearScreen();
	//LCD_vSend_string("AC turn on");
	SET_BIT(data_room,ac_status_bit);
	ac_control();
}
void ac_turnOff()
{
	LCD_ClearScreen();
	//LCD_vSend_string("AC turn off");
	CLR_BIT(data_room,ac_status_bit);
	ac_control();
}
void ac_temp()
{
	LCD_ClearScreen();
	LCD_vSend_string("Set temperature:");
	LCD_movecursor(2,1);
	char x,c=0,temp=0;
	while(c<2)
	{
	L:x=keypad_u8check_press();
	_delay_ms(200);
		while (x==NOTPRESSED)
		{
			x=keypad_u8check_press();
			_delay_ms(200);
		}
		if(x>=48 && x<=57)
		{
			if(c==0)
			{
				temp=(x-48)*10;
				c++;
			}
			else if(c==1)
			{
				temp=temp+(x-48);
				c++;
			}
			LCD_vSend_char(x);
		}
		else if(x=='A')
			ac();
	}
	if(temp>30)
		temp=30;
	else if(temp<18)
		temp=18;
	temp=temp-18;
	for(int i=0;i<4;i++)
	{
		
		CLR_BIT(data_room,ac_temp_bit+i);
	}
	data_room=(data_room)|(temp<<ac_temp_bit);
	_delay_ms(200);
	ac();
}
void display_sys()
{
	LCD_ClearScreen();
	LCD_vSend_string("1:Set time");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A')
		admin();
	else if(x=='1')
	{
		set_clk();
	}
	else 
		goto L;
}
void set_clk()
{
	char first_digit,second_digit;
	H:LCD_ClearScreen();
	LCD_vSend_string("Hours=--");
	_delay_ms(200);
	LCD_movecursor(1,7);
	//Getting the hour tens digit
	do
	{
		first_digit=keypad_u8check_press();
	} while (first_digit==NOTPRESSED);
	//Making sure that the tens digit is not more than 2
	if(first_digit=='A')
		admin();
	if ((first_digit-48)>2)
	{
		LCD_ClearScreen();
		LCD_vSend_string("Invalid Number");
		_delay_ms(500);
		goto H;
	}
	LCD_vSend_char(first_digit);
	_delay_ms(200);
	//Getting the hour ones digit
	do
	{
		second_digit=keypad_u8check_press();
	} while (second_digit==NOTPRESSED);
	if(second_digit=='A')
		admin();
	//Making sure that the hours is not more than 24
	if (((second_digit-48)+10*(first_digit-48))>24)
	{
		LCD_ClearScreen();
		LCD_vSend_string("Invalid Number");
		_delay_ms(500);
		goto H;
	}
	LCD_vSend_char(second_digit);
	_delay_ms(200);
	hours=(second_digit-48)+10*(first_digit-48); //Calculating hours
	// Preparing to receive the minutes
	M:LCD_ClearScreen();
	LCD_vSend_string("Minutes=--");
	LCD_movecursor(1,9);
	//Getting the minutes tens digit
	do
	{
		first_digit=keypad_u8check_press();
	} while (first_digit==NOTPRESSED);
	if(first_digit=='A')
		admin();
	//Making sure that the tens digit is not more than 6
	if ((first_digit-48)>6)
	{
		LCD_ClearScreen();
		LCD_vSend_string("Invalid Number");
		_delay_ms(500);
		goto M;
	}

	LCD_vSend_char(first_digit);
	_delay_ms(200);
	//Getting the minutes ones digit
	do
	{
		second_digit=keypad_u8check_press();
	} while (second_digit==NOTPRESSED);
	if(second_digit=='A')
		admin();
	//Making sure that the minutes is not more than 60
	if (((second_digit-48)+10*(first_digit-48))>60)
	{
		LCD_ClearScreen();
		LCD_vSend_string("Invalid Number");
		_delay_ms(500);
		goto M;
	}
	LCD_vSend_char(second_digit);
	_delay_ms(200);
	minutes=(second_digit-48)+10*(first_digit-48); //Calculating minutes
	// Preparing to receive the seconds
	S:LCD_ClearScreen();
	LCD_vSend_string("Seconds=--");
	LCD_movecursor(1,9);
	//Getting the minutes tens digit
	do
	{
		first_digit=keypad_u8check_press();
	} while (first_digit==NOTPRESSED);
	if(first_digit=='A')
		admin();
	//Making sure that the tens digit is not more than 6
	if ((first_digit-48)>6)
	{
		LCD_ClearScreen();
		LCD_vSend_string("Invalid Number");
		_delay_ms(500);
		goto S;
	}
	LCD_vSend_char(first_digit);
	_delay_ms(200);
	//Getting the minutes ones digit
	do
	{
		second_digit=keypad_u8check_press();
	} while (second_digit==NOTPRESSED);
	if(second_digit=='A')
		admin();
	//Making sure that the seconds is not more than 60
	if (((second_digit-48)+10*(first_digit-48))>60)
	{
		LCD_ClearScreen();
		LCD_vSend_string("Invalid Number");
		_delay_ms(500);
		goto S;
	}
	LCD_vSend_char(second_digit);
	_delay_ms(200);
	LCD_ClearScreen();
	seconds=(second_digit-48)+10*(first_digit-48); //Calculating seconds
	SET_BIT(hours,5);
	SET_BIT(minutes,6);
	SET_BIT(seconds,7);
	time_flag=1;
	admin();
}
void garage_sys(char mode)
{
	LCD_ClearScreen();
	LCD_vSend_string("1:Open garage");
	LCD_movecursor(2,1);
	LCD_vSend_string("2:Close garage");
	char x;
	L:x=keypad_u8check_press();
	_delay_ms(200);
	while (x==NOTPRESSED)
	{
		x=keypad_u8check_press();
		_delay_ms(200);
	}
	if(x=='A' && mode==2)
		admin();
	if(x=='A' && mode==1)
		guest();
	else if(x=='1')
	{
		garage_open(mode);
	}
	else if(x=='2')
	{
		garage_close(mode);
	}
	else
	goto L;
}
void garage_open(char mode)
{
	LCD_ClearScreen();
	LCD_vSend_string("Garage Opened");
	SET_BIT(guest_flag,1);
	SET_BIT(TIMSK,OCIE0);
	_delay_ms(300);
	if(mode==2)
	admin();
	else if (mode==1)
	guest();
}
void garage_close(char mode)
{
	LCD_ClearScreen();
	LCD_vSend_string("Garage Closed");
	CLR_BIT(guest_flag,1);
	CLR_BIT(TIMSK,OCIE0);
	_delay_ms(300);
	if(mode==2)
		admin();
	else if (mode==1)
		guest();
}
void guest()
{
	LCD_ClearScreen();
	garage_sys(1);
}
int main(void)
{
	init();
	welcome();
	char z=(EEPROM_read(sys_hack_check1)+EEPROM_read(sys_hack_check2)+EEPROM_read(sys_hack_check3));
	if(EEPROM_read(sys_en_status)==0xff || z!=sys_hack_val)
		sys_block();
	
	check_first_time();
	choose_mode();
}
ISR(INT0_vect)
{
	SPI_SlaveReceivechar(data_room);
	SPI_SlaveReceivechar(guest_flag);
	if(time_flag==1)
	{SPI_SlaveReceivechar(hours);
	SPI_SlaveReceivechar(minutes);
	SPI_SlaveReceivechar(seconds);
	time_flag=0;}
	else 
	{
		SPI_SlaveReceivechar(0);
		SPI_SlaveReceivechar(0);
		SPI_SlaveReceivechar(0);
	}
	
}
ISR(TIMER0_COMP_vect)
{
	counter++;
	if(counter==1000)
	{
		counter=0;
		CLR_BIT(guest_flag,1);
		CLR_BIT(TIMSK,OCIE0);
	}
}