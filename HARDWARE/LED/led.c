#include "led.h"
#include "mqtt.h"
#include "string.h"
#include "stdio.h" 

//初始化PB5和PE5为输出口.并使能这两个口的时钟
//LED IO初始化
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
//    GPIO_ResetBits(GPIOC,GPIO_Pin_13);

    LED1=0;

}



void LED_StateRefresh(uint8_t LEDState)  
{  
#define LED_PERIID_TIME       10    // period 1.0s  
#define LED_ON_TIME     1     // lighting last time  
#define LED_CYCLE_TIME        1     // one times
      
    static int32_t s_LedCycleTime = LED_PERIID_TIME;  
    static int32_t s_Cnt = LED_ON_TIME * LED_CYCLE_TIME * 2;      
  
    if(s_LedCycleTime > 0){  
        s_LedCycleTime --;  
        if(s_Cnt > 0){  
            if(s_Cnt % (LED_ON_TIME * 2) == 0){  
                LED1=1;
            }else{  
                LED1=0;
            }  
            s_Cnt --;  
        }else{  
            LED1=0;
        }  
    }else{  
        s_LedCycleTime = LED_PERIID_TIME;  
        s_Cnt = LED_ON_TIME * LEDState * 2;  
        LED1=0;
    }  
}


