#ifndef __BEEP_H
#define	__BEEP_H


#include "sys.h" 


/* 相关端口配置宏 */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define BEEP_GPIO_PORT    	GPIOA						/* GPIO端口 */
#define BEEP_GPIO_PIN		GPIO_Pin_8					/* 连接到SCL时钟线的GPIO */


/* 定义控制IO的宏 */
#define BEEP_TOGGLE		 digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON		   GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF			   GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)


void BEEP_Init(void);
void Beep_StateRefresh(uint8_t BeepState); 





#endif 
