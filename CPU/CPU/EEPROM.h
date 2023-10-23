/*
 * EEPROM.h
 *
 * Created: 10/21/2021 1:08:26 PM
 *  Author: Abdulrhman Nasser
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
unsigned char EEPROM_read (unsigned short adress);
void EEPROM_write(unsigned short address,unsigned char data );




#endif /* EEPROM_H_ */