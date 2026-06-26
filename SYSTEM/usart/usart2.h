#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"
#include "sys.h"


extern u16 USART2_RxCounter;
extern char USART2_RxBuff[1024];
extern float TempMax,TempMin;
void Usart2_Init(unsigned int baud);
uint8_t CHeck(uint8_t *data);
void GetTemperature_GY90640(void);
void u2_printf(char* fmt,...) ;
void u2_TxData(unsigned char *data);
#endif


