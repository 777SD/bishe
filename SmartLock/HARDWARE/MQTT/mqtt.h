#ifndef __MQTT_H__
#define __MQTT_H__

#include "sys.h"

#define  DEVICENAME      "D001" 
#define  PRODUCTKEY      "a1gmuLcjBOt"
#define  DEVICESECRE     "d0247636f9e25cc17b4e1fccab4c69e6" 
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)

#define  SUBSCRIBE_TOPIC         "/SmartElectCommunityLock0806/Sub"   
#define  P_TOPIC_NAME            "/SmartElectCommunityLock0806/Pub"    //需要发布的主题    

#define  CLIENT_ID      	"mqttx_39d0806" 
#define  USERNAME      		"root"
#define  MQTT_PASSWORD    "root" 
#define  SERVER_IP				"47.109.89.8"



#define  mqtt_TxData(x)       u2_TxData(x) 

extern char ServerIP[128];                                  
extern int  ServerPort;

extern unsigned char  mqtt_TxBuf[7][400];           
extern unsigned char *mqtt_TxInPtr;                            
extern unsigned char *mqtt_TxOutPtr;                           
extern unsigned char *mqtt_TxEndPtr;

extern unsigned char  mqtt_RxBuf[7][400];           
extern unsigned char *mqtt_RxInPtr;                            
extern unsigned char *mqtt_RxOutPtr;                           
extern unsigned char *mqtt_RxEndPtr;

extern unsigned char  mqtt_CMDBuf[7][400];              
extern unsigned char *mqtt_CMDInPtr;                               
extern unsigned char *mqtt_CMDOutPtr;                              
extern unsigned char *mqtt_CMDEndPtr; 

extern u8 Connect_flag;
extern u8 ConnectPack_flag; 
extern u8 SubscribePack_flag;
extern u8 Ping_flag;
void Mqtt_ConnectMessege(void);
void Ali_MsessageInit(void);
void MQTT_Buff_Init(void);
void mqtt_Ping(void);
void mqtt_PublishQs0(char *topic, char *data, int data_len);
void mqtt_Dealsetdata_Qs0(unsigned char *redata);
void Connect_Server(void);
void Mqtt_uploads_Sensor_Data(void);
char * mid(char *dst,char *src, int n,int m); /*n为长度，m为位置*/
#endif

