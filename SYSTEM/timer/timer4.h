#ifndef __TIMER4_H
#define __TIMER4_H
#include "sys.h"


void TIM4_Int_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc,u8 ch,uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pinx);
#endif
