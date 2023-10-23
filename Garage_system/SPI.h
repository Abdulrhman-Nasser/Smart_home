/*
 * SPI.h
 *
 * Created: 8/3/2023 12:10:55 PM
 *  Author: Abdulrhman Nasser
 */ 


#ifndef SPI_H_
#define SPI_H_
void SPI_MasterInit(void);
void SPI_SlaveInit(void);
unsigned char SPI_MasterTransmitchar(unsigned char Data);
unsigned char SPI_SlaveReceivechar(unsigned char Data);
void SPI_MasterTransmitstring(unsigned char *ptr);

#endif /* SPI_H_ */