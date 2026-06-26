#include "matrixkeys.h"
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "main.h"
#include "key.h"

u8 isKey0 = 0;//数字0 s10
u8 isKey1 = 0;//数字1 s1
u8 isKey2 = 0;//数字2 s2
u8 isKey3 = 0;//数字3 s3
u8 isKey4 = 0;//数字4 s4
u8 isKey5 = 0;//数字5 s5
u8 isKey6 = 0;//数字6 s6
u8 isKey7 = 0;//数字7 s7
u8 isKey8 = 0;//数字8 s8
u8 isKey9 = 0;//数字9 s9
u8 isKeyA = 0;//翻页 s13
u8 isKeyB = 0;//选择 s14
u8 isKeyC = 0;//确认 s15
u8 isKeyD = 0;//返回(在主页面时按下表示进入密码开锁) s16
u8 isKeyE = 0;//加 s11
u8 isKeyF = 0;//减 s12

void KEY1_4_GPIO_Input_Config(void)
{
	GPIO_InitTypeDef Init_Structure;
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK,ENABLE);
	
	Init_Structure.GPIO_Mode = GPIO_Mode_IPD;
	Init_Structure.GPIO_Pin = KEY1_GPIO_Pin;
 
	GPIO_Init(KEY1_PORT,&Init_Structure);
	
	Init_Structure.GPIO_Pin = KEY2_GPIO_Pin;
	GPIO_Init(KEY2_PORT,&Init_Structure);
 
	Init_Structure.GPIO_Pin = KEY3_GPIO_Pin;
	GPIO_Init(KEY3_PORT,&Init_Structure);
 
	Init_Structure.GPIO_Pin = KEY4_GPIO_Pin;
	GPIO_Init(KEY4_PORT,&Init_Structure);
}
 
void KEY5_8_GPIO_Input_Config(void)
{
	GPIO_InitTypeDef Init_Structure;
	RCC_APB2PeriphClockCmd(KEY5_GPIO_CLK|KEY6_GPIO_CLK|KEY7_GPIO_CLK|KEY8_GPIO_CLK,ENABLE);
	
	Init_Structure.GPIO_Mode = GPIO_Mode_IPD;
	Init_Structure.GPIO_Pin = KEY5_GPIO_Pin;
 
	GPIO_Init(KEY5_PORT,&Init_Structure);
	
	Init_Structure.GPIO_Pin = KEY6_GPIO_Pin;
	GPIO_Init(KEY6_PORT,&Init_Structure);
 
	Init_Structure.GPIO_Pin = KEY7_GPIO_Pin;
	GPIO_Init(KEY7_PORT,&Init_Structure);
 
	Init_Structure.GPIO_Pin = KEY8_GPIO_Pin;
	GPIO_Init(KEY8_PORT,&Init_Structure);
}
 
void KEY1_4_GPIO_Output_Config(void)
{
	GPIO_InitTypeDef Init_Structure;
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK,ENABLE);
	
	Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	Init_Structure.GPIO_Pin = KEY1_GPIO_Pin;
 
	GPIO_Init(KEY1_PORT,&Init_Structure);
	
	Init_Structure.GPIO_Pin = KEY2_GPIO_Pin;
	GPIO_Init(KEY2_PORT,&Init_Structure);
 
	Init_Structure.GPIO_Pin = KEY3_GPIO_Pin;
	GPIO_Init(KEY3_PORT,&Init_Structure);
 
	Init_Structure.GPIO_Pin = KEY4_GPIO_Pin;
	GPIO_Init(KEY4_PORT,&Init_Structure);
    
    GPIO_SetBits(KEY1_PORT,KEY1_GPIO_Pin);
	GPIO_SetBits(KEY2_PORT,KEY2_GPIO_Pin);
	GPIO_SetBits(KEY3_PORT,KEY3_GPIO_Pin);
	GPIO_SetBits(KEY4_PORT,KEY4_GPIO_Pin);
}
 
void KEY5_8_GPIO_Output_Config(void)
{
	GPIO_InitTypeDef Init_Structure;
	RCC_APB2PeriphClockCmd(KEY5_GPIO_CLK|KEY6_GPIO_CLK|KEY7_GPIO_CLK|KEY8_GPIO_CLK,ENABLE);
    
	Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	Init_Structure.GPIO_Pin = KEY5_GPIO_Pin;
 
	GPIO_Init(KEY5_PORT,&Init_Structure);
	
	Init_Structure.GPIO_Pin = KEY6_GPIO_Pin;
	GPIO_Init(KEY6_PORT,&Init_Structure);
 
	Init_Structure.GPIO_Pin = KEY7_GPIO_Pin;
	GPIO_Init(KEY7_PORT,&Init_Structure);
 
	Init_Structure.GPIO_Pin = KEY8_GPIO_Pin;
	GPIO_Init(KEY8_PORT,&Init_Structure);
    
    GPIO_SetBits(KEY5_PORT,KEY5_GPIO_Pin);
	GPIO_SetBits(KEY6_PORT,KEY6_GPIO_Pin);
	GPIO_SetBits(KEY7_PORT,KEY7_GPIO_Pin);
	GPIO_SetBits(KEY8_PORT,KEY8_GPIO_Pin);
}
 
/**
  * @brief 读取KEY的GPIO引脚状态（这个函数这里没用到，但为了保持完整性还是贴出来）
  * @param GPIO,Pin
  * @retval 引脚状态
  */
uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)== KEY_ON)
	{
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)== KEY_ON)
			return KEY_ON;
	}
	return KEY_OFF;
}
/**
  * @brief 给1234引脚置高电平
  * @param void
  * @retval void
  */
void set_key1_4_Bits(void)
{
	GPIO_SetBits(KEY1_PORT,KEY1_GPIO_Pin);
	GPIO_SetBits(KEY2_PORT,KEY2_GPIO_Pin);
	GPIO_SetBits(KEY3_PORT,KEY3_GPIO_Pin);
	GPIO_SetBits(KEY4_PORT,KEY4_GPIO_Pin);
}
 
/**
  * @brief 读取1234引脚电平并存到temp低四位
  * @param void
  * @retval 返回4个引脚电平状态
  */
u8 read_key1_4_Bits(void)
{
	u8 k1=0,k2=0,k3=0,k4=0,temp = 0;
	k1 = GPIO_ReadInputDataBit(KEY1_PORT,KEY1_GPIO_Pin);
	temp |= k1;
	
	k2 = GPIO_ReadInputDataBit(KEY2_PORT,KEY2_GPIO_Pin);
	k2<<=1;
	temp |= k2;
	
	k3 = GPIO_ReadInputDataBit(KEY3_PORT,KEY3_GPIO_Pin);
	k3<<=2;
	temp |= k3;
	
	k4 = GPIO_ReadInputDataBit(KEY4_PORT,KEY4_GPIO_Pin);
	k4<<=3;
	temp |= k4;
	return temp;
}
 
/**
  * @brief 给5678引脚置高电平
  * @param void
  * @retval void
  */
void set_key5_8_Bits(void)
{
	GPIO_SetBits(KEY5_PORT,KEY5_GPIO_Pin);
	GPIO_SetBits(KEY6_PORT,KEY6_GPIO_Pin);
	GPIO_SetBits(KEY7_PORT,KEY7_GPIO_Pin);
	GPIO_SetBits(KEY8_PORT,KEY8_GPIO_Pin);
}
 
/**
  * @brief 读取5678引脚电平并存到temp高四位
  * @param void
  * @retval 返回4个引脚电平状态
  */
u8 read_key5_8_Bits(void)
{
	u8 k5=0,k6=0,k7=0,k8=0,temp = 0;
	k5 = GPIO_ReadInputDataBit(KEY5_PORT,KEY5_GPIO_Pin);
	k5<<=4;
	temp |= k5;
	
	k6 = GPIO_ReadInputDataBit(KEY6_PORT,KEY6_GPIO_Pin);
	k6<<=5;
	temp |= k6;
	
	k7 = GPIO_ReadInputDataBit(KEY7_PORT,KEY7_GPIO_Pin);
	k7<<=6;
	temp |= k7;
	
	k8 = GPIO_ReadInputDataBit(KEY8_PORT,KEY8_GPIO_Pin);
	k8<<=7;
	temp |= k8;
	
	return temp;
}
 
/**
  * @description 获取按键的值以确定是哪个按钮被按下
  * @param void
  *
  * @retval 0代表未被按下，非0表示被按下的按钮的值
  *
  *
  */
u8 get_button_value(void)
{
	u8 button_value = 0;
	u8 k1_4=0,k5_8=0;
	KEY1_4_GPIO_Output_Config();    //设置输出模式
	KEY5_8_GPIO_Input_Config();     //设置输入模式
 
	set_key1_4_Bits();		            //1234引脚置高电平
//	delay_ms(1);                           //延时1ms确保电平稳定	
	k5_8 = read_key5_8_Bits();          //读取5678引脚电平
//	printf("k5_8=0x%2x\r\n",k5_8);
	/*如果不等于0说明有按键按下*/
	if(k5_8!=0)					
	{
		delay_ms(5);	                    //延时消抖动，再次予以确认
		k5_8 = read_key5_8_Bits();
		
		if(k5_8!=0)
		{
			KEY1_4_GPIO_Input_Config(); //设置输入模式
			KEY5_8_GPIO_Output_Config();//设置输出模式
			delay_ms(1);	                //延时1ms确保电平稳定	
			set_key5_8_Bits();
 
			k1_4=read_key1_4_Bits();			
			delay_ms(5);                  //延时消抖动，再次予以确认			
			k1_4=read_key1_4_Bits();
			///生成按键值
			button_value |= k5_8;      //实际上只取了k5_8高四位
			button_value |= k1_4;      //实际上只取了k1_4低四位
			return button_value;
		}
	}
	return 0;
}


/*
******按键扫描函数*****************************************
*	功能:	按键扫描函数
*	入口参数:	无
*	返回参数:	无
*	说明:		KEY1-PB14		KEY2-PB12   KEY3-PB15    KEY4-PB13
************************************************************
*/
void MatrixScan(void)
{
    static int keyCount = 0;
    static int keyState = 0;
   Key.KeyNum = get_button_value();
    if(keyState == 0)
    {
//        printf("Key.KeyNum:%x\r\n",Key.KeyNum);
        switch(Key.KeyNum)
        {
            case 0x88:
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x88 && keyState == 0)
                       {
                           keyState=1;
                           Key1Press();     
                       }break;
            case 0x84: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x84 && keyState == 0)
                       {
                           keyState=1;
                           Key2Press();     
                       }break;
            case 0x82: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x82 && keyState == 0)
                       {
                           keyState=1;
                           Key3Press();     
                       }break;   
            case 0x81:
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x81 && keyState == 0)
                       {
                           keyState=1;
                           Key4Press();     
                       }break;
            case 0x48:
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x48 && keyState == 0)
                       {
                           keyState=1;
                           Key5Press();     
                       }break;
            case 0x44: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x44 && keyState == 0)
                       {
                           keyState=1;
                           Key6Press();     
                       }break;
            case 0x42: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x42 && keyState == 0)
                       {
                           keyState=1;
                           Key7Press();     
                       }break;
            case 0x41: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x41 && keyState == 0)
                       {
                           keyState=1;
                           Key8Press();     
                       }break;
                       
            case 0x28:
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x28 && keyState == 0)
                       {
                           keyState=1;
                           Key9Press();     
                       }break;
            case 0x24: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x24 && keyState == 0)
                       {
                           keyState=1;
                           Key0Press();     
                       }break;
            case 0x22: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x22 && keyState == 0)
                       {
                           keyState=1;
                           KeyEPress();     
                       }break;
            case 0x21: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x21 && keyState == 0)
                       {
                           keyState=1;
                           KeyFPress();     
                       }break;
            case 0x18:
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x18 && keyState == 0)
                       {
                           keyState=1;
                           KeyAPress();     
                       }break;
            case 0x14: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x14 && keyState == 0)
                       {
                           keyState=1;
                           KeyBPress();     
                       }break;
            case 0x12: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x12 && keyState == 0)
                       {
                           keyState=1;
                           KeyCPress();     
                       }break; 

            case 0x11: 
                       keyCount++;
                       if(keyCount > 2 && Key.KeyNum == 0x11 && keyState == 0)
                       {
                           keyState=1;
                           KeyDPress();     
                       }break;                       
        }
    }
    else if(keyState==1&&Key.KeyNum==0x00)
    {
        keyCount = 0;
        keyState = 0;
    }

}

void Key0Press(void)
{
    if(isKey0 == 0)
        isKey0=1;
}

void Key1Press(void)
{
    if(isKey1 == 0)
        isKey1=1;
}

void Key2Press(void)
{
    if(isKey2 == 0)
        isKey2=1;

}

void Key3Press(void)
{
    if(isKey3 == 0)
        isKey3=1;

}

void Key4Press(void)
{
    if(isKey4 == 0)
        isKey4=1;

}

void Key5Press(void)
{
    if(isKey5 == 0)
        isKey5=1;
}

void Key6Press(void)
{
    if(isKey6 == 0)
        isKey6=1;

}

void Key7Press(void)
{
    if(isKey7 == 0)
        isKey7=1;

}

void Key8Press(void)
{
    if(isKey8 == 0)
        isKey8=1;

}

void Key9Press(void)
{
    if(isKey9 == 0)
        isKey9=1;

}

void KeyAPress(void)
{
    if(isKeyA == 0)
        isKeyA=1;

}

void KeyBPress(void)
{
    if(isKeyB == 0)
        isKeyB=1;

}

void KeyCPress(void)
{
    if(isKeyC == 0)
        isKeyC=1;

}

void KeyDPress(void)
{
    if(isKeyD == 0)
        isKeyD=1;

}

void KeyEPress(void)
{
    if(isKeyE == 0)
        isKeyE=1;

}

void KeyFPress(void)
{
    if(isKeyF == 0)
        isKeyF=1;

}

void KeyState_Clear(void)
{
     isKey0 = 0;//数字0 s10
     isKey1 = 0;//数字1 s1
     isKey2 = 0;//数字2 s2
     isKey3 = 0;//数字3 s3
     isKey4 = 0;//数字4 s4
     isKey5 = 0;//数字5 s5
     isKey6 = 0;//数字6 s6
     isKey7 = 0;//数字7 s7
     isKey8 = 0;//数字8 s8
     isKey9 = 0;//数字9 s9
     isKeyA = 0;//数字9 s11   
     isKeyB = 0;//数字9 s12
     isKeyC = 0;//数字9 s13
     isKeyD = 0;//数字9 s14
     isKeyE = 0;//数字9 s15
     isKeyF = 0;//数字9 s16
}
