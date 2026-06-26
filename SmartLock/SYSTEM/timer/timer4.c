#include "timer4.h"
#include "led.h"
#include "usart.h"
#include "usart2.h"
#include "as608.h"
#include "mqtt.h"
#include "string.h"
/******************   	  
功能：通用定时器2中断初始化,这里时钟选择为APB1的2倍，而APB1为36M
参数：arr：自动重装值。
			psc：时钟预分频数
返回值：无
******************/
void TIM4_Int_Init(u16 arr,u16 psc)
{  
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	
    
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update); 
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, DISABLE);
							 
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  
	{
        
        
		memcpy(&mqtt_RxInPtr[2],USART2_RxBuff,USART2_RxCounter);  
		mqtt_RxInPtr[0] = USART2_RxCounter/256;                   
		mqtt_RxInPtr[1] = USART2_RxCounter%256;                   
		mqtt_RxInPtr+=400;                                  
		if(mqtt_RxInPtr==mqtt_RxEndPtr)                       
			mqtt_RxInPtr = mqtt_RxBuf[0]; 
		
		USART2_RxCounter = 0;                                         
		TIM_Cmd(TIM4, DISABLE);                        				 
		TIM_SetCounter(TIM4, 0);                        			  
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 					
	}

}

/******************

注意！！！
1.当不使用重映射时，默认TIM4四个的IO口是PB6、PB7、PB8、PB9

2.要使用PD12、PD13、PD14、PD15的端口组合，要调用下面的语句进行部分重映射：
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //重映射必须要开AFIO时钟
  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM4, ENABLE);


功能：TIM4 PWM部分初始化，初始化引脚使其输出PWM信号
参数：arr：自动重装值
			psc：时钟预分频数
			ch: 定时器通道号 范围：1--4
			RCC_APB2Periph: APB2对应的gpio端口号 如：RCC_APB2Periph_GPIOB，RCC_APB2Periph_GPIOC
			GPIOx: GPIO端口 如GPIOB
			GPIO_Pin：GPIO号 如GPIO_Pin_5
返回值：无
应用示例：TIM4_PWM_Init(4999,7199,2,RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_5);
******************/
void TIM4_PWM_Init(u16 arr,u16 psc,u8 ch,uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pinx)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
/**********************************重映射配置*********************************************/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer4重映射 
/**********************************非重映射配置*********************************************/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph| RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
 
   //设置该引脚为复用输出功能,输出TIM4 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	if(ch==1)
	{
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC1
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	}
	else if(ch==2)
	{
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	}
	else if(ch==3)
	{
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC3
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	}
	else if(ch==4)
	{
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC4
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	}
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4

}
