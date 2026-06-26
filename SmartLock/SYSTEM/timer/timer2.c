#include "timer2.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "as608.h"
#include "mqtt.h" 
#include "usart3.h"

#include "matrixkeys.h"
#include "main.h"
/******************   	  
功能：通用定时器2中断初始化,这里时钟选择为APB1的2倍，而APB1为36M
参数：arr：自动重装值。
			psc：时钟预分频数
返回值：无
******************/

uint16_t TimeCount1=0,TimeCount2=0;
uint16_t gMsCount=0;
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	
	TIM_Cmd(TIM2, ENABLE);  //失能TIM2						 
}

//定时器2中断服务程序
uint8_t Keynum=0;
void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
            MatrixScan();//矩阵键盘扫描
//            printf("Keynum:0x%2x\r\n",Key.KeyNum);
            if(SystemData.ClearFlag)//若清除oled某一行的标志为1
            {
                TimeCount1++;//开始计数
                if(TimeCount1>=40) SystemData.ClearFlag=TimeCount1=0;//大于等于40时，将标志位置零
            }
            TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
		}
}



/*
 *==============================================================================
功能：TIM2 PWM部分初始化，初始化引脚使其输出PWM信号
参数：arr：自动重装值
			psc：时钟预分频数
			ch: 定时器通道号 范围：1--4
			RCC_APB2Periph: APB2对应的gpio端口号 如：RCC_APB2Periph_GPIOA，RCC_APB2Periph_GPIOC
			GPIOx: GPIO端口 如GPIOA
			GPIO_Pin：GPIO号 如GPIO_Pin_2
返回值：无
应用示例：TIM2_PWM_Init(4999,7199,3,RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_2);

注意！！！
1.当不使用重映射时，默认TIM2四个的IO口是PA0、PA1、PA2、PA3

2.要使用PA15、PB3、PA2、PA3的端口组合，要调用下面的语句进行部分重映射：
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //重映射必须要开AFIO时钟
  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);

3.要使用PA0、PA1、PB10、PB11的端口组合，要调用下面的语句进行部分重映射：
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //重映射必须要开AFIO时钟
  GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);

4.要使用PA15、PB3、PB10、PB11的端口组合，要调用下面的语句进行完全重映射：
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //重映射必须要开AFIO时钟
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//禁止JTAG功能，把PB3，PB4作为普通IO口使用
同时还要禁用JTAG功能，PA15、PB3、PB10、PB11才会正常输出。
 *==============================================================================
 */
void TIM2_PWM_Init(u16 arr,u16 psc,u8 ch,uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pinx)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); //Timer3部分重映射  TIM2_CH2->PB5 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //重映射必须要开AFIO时钟
//    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//禁止JTAG功能，把PB3，PB4作为普通IO口使用
 
   //设置该引脚为复用输出功能,输出TIM2 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	if(ch==1)
	{
		TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	}
	else if(ch==2)
	{
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC2
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	}
	else if(ch==3)
	{
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC3
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	}
	else if(ch==4)
	{
		TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC4
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	}
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
	//TIM_Cmd(TIM2, DISABLE);  //失能TIM2
}

/*
 *==============================================================================
 *函数名称：OpenTimer2
 *函数功能：打开定时器2,并将计数值清零
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
void OpenTimer2(void)   // 打开定时器  
{  
	TIM_SetCounter(TIM2,0);   // 清除计数  
	gMsCount = 0;  
	TIM_Cmd(TIM2, ENABLE);   // 使能TIMx外设  
}
/*
 *==============================================================================
 *函数名称：CloseTimer2
 *函数功能：关闭定时器2
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
void CloseTimer2(void)   // 关闭定时器  
{  
	TIM_Cmd(TIM2, DISABLE);   // 使能TIMx外设  
}  


/*
 *==============================================================================
 *函数名称：GetEchoTimer
 *函数功能：获取定时器定时时间 
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
u32 GetEchoTimer(void)  
{  
	u32 t = 0;  
	t = gMsCount * 1000;   // 得到MS  
	t += TIM_GetCounter(TIM2);   // 得到US  
	TIM2 -> CNT = 0;   // 将TIM2计数寄存器的计数值清零  
	delay_ms(50);
	return t;  
}
