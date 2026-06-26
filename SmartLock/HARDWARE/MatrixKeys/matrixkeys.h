#ifndef __MATRIXKEYS_H
#define	__MATRIXKEYS_H
#include "sys.h" 

extern u8 isKey0;
extern u8 isKey1;
extern u8 isKey2;
extern u8 isKey3;
extern u8 isKey4;
extern u8 isKey5;
extern u8 isKey6;
extern u8 isKey7;
extern u8 isKey8;
extern u8 isKey9;
extern u8 isKeyA;
extern u8 isKeyB;
extern u8 isKeyC;
extern u8 isKeyD;
extern u8 isKeyE;
extern u8 isKeyF;

/*定义需要的GPIO引脚,KEY1代表电路图中的引脚1，依此类推*/
#define KEY1_PORT 			GPIOB 
#define KEY1_GPIO_Pin		GPIO_Pin_5
#define KEY1_GPIO_CLK		RCC_APB2Periph_GPIOB
 
#define KEY2_PORT 			GPIOB 
#define KEY2_GPIO_Pin		GPIO_Pin_6
#define KEY2_GPIO_CLK		RCC_APB2Periph_GPIOB
 
#define KEY3_PORT 			GPIOB 
#define KEY3_GPIO_Pin		GPIO_Pin_7
#define KEY3_GPIO_CLK		RCC_APB2Periph_GPIOB
 
#define KEY4_PORT 			GPIOB 
#define KEY4_GPIO_Pin		GPIO_Pin_8
#define KEY4_GPIO_CLK		RCC_APB2Periph_GPIOB
 
#define KEY5_PORT 			GPIOB 
#define KEY5_GPIO_Pin		GPIO_Pin_12
#define KEY5_GPIO_CLK		RCC_APB2Periph_GPIOB
 
#define KEY6_PORT 			GPIOB 
#define KEY6_GPIO_Pin		GPIO_Pin_13
#define KEY6_GPIO_CLK		RCC_APB2Periph_GPIOB
 
#define KEY7_PORT 			GPIOB 
#define KEY7_GPIO_Pin		GPIO_Pin_14
#define KEY7_GPIO_CLK		RCC_APB2Periph_GPIOB
 
#define KEY8_PORT 			GPIOB 
#define KEY8_GPIO_Pin		GPIO_Pin_15
#define KEY8_GPIO_CLK		RCC_APB2Periph_GPIOB
 
#define KEY_ON 1
#define KEY_OFF 0
 
/*所有按键的代码值*/
#define KS1_CODE	0x18
#define KS2_CODE	0x14
#define KS3_CODE	0x12
#define KS4_CODE	0x11
#define KS5_CODE	0x28
#define KS6_CODE	0x24
#define KS7_CODE	0x22
#define KS8_CODE	0x21
#define KS9_CODE	0x48
#define KS10_CODE	0x44
#define KS11_CODE	0x42
#define KS12_CODE	0x41
#define KS13_CODE	0x88
#define KS14_CODE	0x84
#define KS15_CODE	0x82
#define KS16_CODE	0x81

void KEY1_4_GPIO_Input_Config(void);
void KEY5_8_GPIO_Input_Config(void);
void KEY1_4_GPIO_Output_Config(void);
void KEY5_8_GPIO_Output_Config(void);
uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
void set_key1_4_Bits(void);
u8 read_key1_4_Bits(void);
void set_key5_8_Bits(void);
u8 read_key5_8_Bits(void);
u8 get_button_value(void);
void MatrixScan(void);
void KeyState_Clear(void);

void Key0Press(void);
void Key1Press(void);
void Key2Press(void);
void Key3Press(void);
void Key4Press(void);

void Key5Press(void);
void Key6Press(void);
void Key7Press(void);
void Key8Press(void);

void Key9Press(void);
void KeyAPress(void);
void KeyBPress(void);
void KeyCPress(void);
void KeyDPress(void);
void KeyEPress(void);
void KeyFPress(void);
#endif 

