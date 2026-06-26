#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"

#include <stdio.h>
#include <string.h> // 添加包含 string.h 头文件
#include "main.h"

SYSTEM SystemData;//系统参数结构体
KEY Key;//按键参数结构体

void OLED_Interface(void);
int main(void)
{
    /////////////////////////////////
    delay_init();//延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    uart_init(115200);	 //串口初始化为115200
    OLED_Init();//OLED初始化 PA11,PA12
    LED_Init();//LED初始化 PC13
    BEEP_Init();//蜂鸣器初始化 PA8
    RC522_Init();
    Usart3_Init(57600);//PB10,PB11 
    As608_TCH_Init();//PB1 触摸感应端口初始化
    TIM1_Int_Init(99,7199);		//10ms中断
    TIM_Cmd(TIM1,DISABLE);			//关闭定时器   
    SG90_Init();//舵机初始化 PB9
    Control_SG90(500);//舵机0°   
    Store_Init();//内部Flash初始化
    memcpy(SetPassWord,&Store_Data[1],7);//读取Flash数据到密码数组
    
//    oled_Clear();//oled清屏
    TIM2_Int_Init(499,7199);//定时器2初始化
    
    while(1)
    {
        Mode_Decide();//模式切换函数
        if(SystemData.LockFlag)//此时若是开锁状态
        {
            SystemData.LockCount++;//开始开锁计时
            LED1=1;
            if(SystemData.LockCount>=300)//若计数大于200
            {
                SystemData.LockCount=0;//计数值清零
                SystemData.LockFlag=0;//开锁状态置零，表示关锁
                Control_SG90(500);//舵机回零(关锁) 
                LED1=0;
            }
        }  

        if(SystemData.WarnFlag)//若触发警报
        {
            isKeyA=isKeyB=isKeyC=isKeyD=isKeyE=isKeyF=0;//先清空按键按下标志
            BEEP_ON;//开启蜂鸣器
            SystemData.WarnCount++;//开始警报计时
            if(SystemData.WarnCount>=500)//若警报计数达到500
            {
                SystemData.WarnCount=0;//清空警报计数
                SystemData.WarnFlag=0;//清除警报标志
                BEEP_OFF;//关闭蜂鸣器  
            }             
        }
            
    }
}


