#ifndef __MAIN_H
#define	__MAIN_H
#include "sys.h"
#include "MyRTC.h" 
#include "servo.h"
#include "mqtt.h"   
#include "bsp_esp8266.h"
#include "rc522.h"
#include "menu.h"
#include "as608.h"
#include "beep.h"
#include "bsp_oled_iic.h"
#include "matrixkeys.h"
#include "timer1.h"
#include "timer2.h"
#include "iic.h"
#include "MyFLASH.h"
#include "Store.h"
//传感器数据结构体
typedef struct
{
    uint16_t WarnCount;//警报计数
    uint8_t PubCount;//消息发布时间间隔
    uint8_t PubFlag;//消息发布标志
    uint16_t LockCount;//关锁时长      
    uint8_t ClearFlag;//清屏标志
    uint8_t LockFlag;//开关锁标志
    uint8_t WarnFlag;//警报标志
}SYSTEM;


typedef struct
{
    uint8_t Ensure;//确认标志
    uint8_t Return;//返回标志
    uint8_t KeyNum;//键值
}KEY;


extern SYSTEM SystemData;//定义系统参数结构体
extern KEY Key;//定义按键结构体

void MQTT_Publish(void);
void MQTT_Subscribe(void);
#endif

