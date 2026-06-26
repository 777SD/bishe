#ifndef __TIMER2_H
#define __TIMER2_H
#include "sys.h"


void TIM2_Int_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc,u8 ch,uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pinx);
void OpenTimer2(void);
void CloseTimer2(void);
u32 GetEchoTimer (void);
#endif

