#include "usart2.h"
#include "string.h"
#include "mqtt.h"
#include "stdarg.h"
#include "usart3.h"
u16 USART2_RxCounter = 0;
char USART2_RxBuff[1024];

/*
******串口2初始化*****************************************
*	功能:	串口2初始化
*	入口参数:	baud:设定波特率
*	返回参数:	无
*	说明:		TX-PA2		RX-PA3
************************************************************
*/
void Usart2_Init(unsigned int baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    //PA2	TXD
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //PA3	RXD
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

}


//校验和检查
void USART2_IRQHandler(void)
{
//    static uint16_t i=0;
//    static	uint16_t receiveLength=0;
    if(Connect_flag == 0)
    {
        if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)  //判断是否发生中断
        {
            if(USART2->DR)
			{                                 	
				USART2_RxBuff[USART2_RxCounter]=USART_ReceiveData(USART2); 	
				USART2_RxCounter++;                        
			}
            USART_ClearITPendingBit(USART2,USART_IT_RXNE);  //清除当前中断标志位

        }
    }
        
    else
    {
        USART2_RxBuff[USART2_RxCounter] =USART_ReceiveData(USART2);
        if(USART2_RxCounter == 0)
        {    									
            TIM_Cmd(TIM4,ENABLE); 
        }else
        {                        					
            TIM_SetCounter(TIM4,0);  
        }	
        USART2_RxCounter++;      
    }
}

__align(8) char USART2_TxBuff[256];  

void u2_printf(char* fmt,...) 
{  
	unsigned int i =0,length=0;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART2_TxBuff,fmt,ap);
	va_end(ap);
	
	length=strlen((const char*)USART2_TxBuff);
	while(i<length)
	{
		Usart_SendByte(USART2,USART2_TxBuff[i]);		
		i++;		
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	
}


void u2_TxData(unsigned char *data)
{
	int	i;
	while(USART_GetFlagStatus( USART2,USART_FLAG_TC) == RESET);
	for(i = 1;i <= (data[0]*256+data[1]);i ++)
	{			
		USART_SendData(USART2,data[i+1]);
		
		while(USART_GetFlagStatus( USART2,USART_FLAG_TC) == RESET);	
	}
}
