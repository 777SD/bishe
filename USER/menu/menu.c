#include "menu.h"

#include "bsp_oled_iic.h"
#include "key.h"
#include "usart3.h"
#include "led.h"
#include "beep.h"

#include "servo.h"
#include "main.h"
#include "as608.h"
#include "delay.h"
#include "rc522.h"
#include "matrixkeys.h"
//static uint8_t ManualFlag=0;
//static uint8_t SetFlag=0;
uint8_t SwitchFlag=0;

/******************   	  
功能：模式选择及控制界面
参数：无
返回值：无
******************/
uint8_t OperateMode=0;//运行模式标志位
void Mode_Decide(void)//模式判定
{
    //按键判定
    if(isKeyA&&!Key.Ensure)//若按键A按下
    {
        isKeyA=0;
        oled_Clear();
        OperateMode++;
    }
    //标志位限制
    if(OperateMode>=3) OperateMode=0;
    /**根据模式选择显示界面**/
    switch(OperateMode)
    {
        case 0:
            AutoContrl();//自动控制
        break;
        
        case 1: 
            Manual_Contrl();
        break;
        
        case 2: 
            CardSet();
        break;
    }
    
}

/******************   	  
功能：显示界面
参数：无
返回值：无
******************/
void Inform_Show(void) 
{
    //智能门锁系统
    oled_ShowCHinese(16,0,0);
    oled_ShowCHinese(32,0,1);
    oled_ShowCHinese(48,0,2);
    oled_ShowCHinese(64,0,3);
    oled_ShowCHinese(80,0,4);
    oled_ShowCHinese(96,0,5);    
    //状态:开锁/关锁
    oled_ShowCHinese(16,4,6);
    oled_ShowCHinese(32,4,7);
    OLED_ShowChar(48,4,':',16);
    if(SystemData.LockFlag)
    {
        oled_ShowCHinese(64,4,43);
        oled_ShowCHinese(80,4,44);        
    }
    else
    {
        oled_ShowCHinese(64,4,8);
        oled_ShowCHinese(80,4,44);         
    }
}


/******************   	  
功能：自动控制界面
参数：无
返回值：无
******************/

void AutoContrl(void)
{
    Inform_Show();
    if(!SystemData.WarnFlag&&!SystemData.LockFlag)
    {
        Verify_CardInformation();//刷卡开锁
        press_FR();//指纹开锁
       if(isKeyB)//密码开锁
        {       
            isKeyB=0;//清除按下标志
            KeyState_Clear();
            Key.Ensure=1;//确认键
            oled_Clear();//清屏
            Verify_Password();//密码开锁 
        }   
    }

}

/******************   	  
功能：手动控制界面
参数：无
返回值：无
******************/
void Manual_Contrl(void)
{
   static uint8_t IndexFlag=1;//光标索引
   uint8_t ensure;
    /*按键检测*/
    if(isKeyB)
    {
        IndexFlag++;
        if(IndexFlag==4) IndexFlag=1;
        isKeyB=0;
    }
    if(isKeyC)
    {
        isKeyC=0;
        Key.Ensure=1;
        KeyState_Clear();
        oled_Clear();
    }
    if(!Key.Ensure)
    {
        //录入指纹
        oled_ShowCHinese(16,0,33);
        oled_ShowCHinese(32,0,34);
        oled_ShowCHinese(48,0,47);
        oled_ShowCHinese(64,0,48);
        //删除指纹
        oled_ShowCHinese(16,2,45);
        oled_ShowCHinese(32,2,46);
        oled_ShowCHinese(48,2,47);
        oled_ShowCHinese(64,2,48);
        //设置密码
        oled_ShowCHinese(16,4,39);
        oled_ShowCHinese(32,4,40);
        oled_ShowCHinese(48,4,41);
        oled_ShowCHinese(64,4,42);
        switch(IndexFlag)//光标显示
        {
            case 1:
                OLED_ShowChar(0,0,'*',16);
                OLED_ShowChar(0,2,' ',16);
                OLED_ShowChar(0,4,' ',16);
            break;
            case 2:
                OLED_ShowChar(0,0,' ',16);
                OLED_ShowChar(0,2,'*',16);
                OLED_ShowChar(0,4,' ',16);
            break;
            case 3:
                OLED_ShowChar(0,0,' ',16);
                OLED_ShowChar(0,2,' ',16);
                OLED_ShowChar(0,4,'*',16);
            break;
        }
    }
    else 
    {
        if(IndexFlag==1) //录入指纹
        {
            Add_FR();
        }
        else if(IndexFlag==2) //删除指纹
        {
            oled_Clear();
            ensure = PS_Empty(); //清空指纹库
            if(ensure == 0)
            {
             oled_ShowString(0, 2, "Delete Success",16);
             delay_ms(100);
            }
            oled_Clear();
            Key.Ensure=0;
        } 
        else if(IndexFlag==3) //设置密码
        {
            Change_Password();
        }

    }
    
    
}

void CardSet(void)
{
    static uint8_t IndexFlag1=1;//光标索引

    
    /*按键检测*/
    if(isKeyB)
    {
        IndexFlag1++;
        if(IndexFlag1==3) IndexFlag1=1;
        isKeyB=0;
    }
    if(isKeyC)
    {
        isKeyC=0;
        Key.Ensure=1;
        oled_Clear();
    }
    if(!Key.Ensure)
    {
        //激活卡片
        oled_ShowCHinese(16,0,49);
        oled_ShowCHinese(32,0,50);
        oled_ShowCHinese(48,0,51);
        oled_ShowCHinese(64,0,52);
        //删除卡片
        oled_ShowCHinese(16,2,57);
        oled_ShowCHinese(32,2,58);
        oled_ShowCHinese(48,2,51);
        oled_ShowCHinese(64,2,52);
        switch(IndexFlag1)//光标显示
        {
            case 1:
                OLED_ShowChar(0,0,'*',16);
                OLED_ShowChar(0,2,' ',16);
                OLED_ShowChar(0,4,' ',16);
            break;
            case 2:
                OLED_ShowChar(0,0,' ',16);
                OLED_ShowChar(0,2,'*',16);
                OLED_ShowChar(0,4,' ',16);
            break;
            case 3:
                OLED_ShowChar(0,0,' ',16);
                OLED_ShowChar(0,2,' ',16);
                OLED_ShowChar(0,4,'*',16);
            break;
        }
    }
    else
    {
        if(IndexFlag1==1) //激活卡片
        {
            Activation_CardInformation();
        }
        else if(IndexFlag1==2)//擦除卡片
        {
            Remove_CardInformation();
        } 
    }
}

