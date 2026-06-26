
#include "sys.h"



#define SG90_GPIO_PIN   GPIO_Pin_9
#define SG90_GPIO_PORT  GPIOB
#define SG90_GPIO_CLK  RCC_APB2Periph_GPIOB


#define SERVO_GPIO_PIN1   GPIO_Pin_0
#define SERVO_GPIO_PIN2   GPIO_Pin_1
#define SERVO_GPIO_PORT  GPIOA
#define SERVO_GPIO_CLK  RCC_APB2Periph_GPIOA


#define SERVO_PIN1 PAout(0)
#define SERVO_PIN2 PAout(1)


void SG90_Init(void);
void Servo_Init(void);
void Servo_Turn(void);
void Servo_Stop(void);
void Control_SG90(uint32_t us);


