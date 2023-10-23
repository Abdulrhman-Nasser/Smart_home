/*
 * Buttons.h
 *
 * Created: 10/3/2021 3:34:49 PM
 *  Author: Abdulrhman Nasser
 */


#ifndef BUTTONS_H_
#define BUTTONS_H_

void Button_vInit(unsigned char portname, unsigned char pinnumber);
unsigned char Button_u8read(unsigned char portname, unsigned char pinnumber);
void Button_InternalPullUp(unsigned char portname, unsigned char pinnumber, unsigned char enable);



#endif /* BUTTONS_H_ */
