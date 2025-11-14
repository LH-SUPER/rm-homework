#include "SoftIIC.h"

static void IIC_Delay(void)
{
for(volatile int i = 0; i < 200; i++);
}

#define SDA_HIGH() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
#define SDA_LOW() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
#define SCL_HIGH() HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET)
#define SCL_LOW() HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)
#define SDA_READ() HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)

void IIC_Start(void)
{
SDA_HIGH();
SCL_HIGH();
IIC_Delay();
SDA_LOW(); 
IIC_Delay();
SCL_LOW();
IIC_Delay();
}

void IIC_Stop(void)
{
SDA_LOW();
IIC_Delay();
SCL_HIGH();
IIC_Delay();
SDA_HIGH();
IIC_Delay();
}

void IIC_SendByte(uint8_t data)
{
for(int i=0; i<8; i++)
{
if(data & 0x80)
SDA_HIGH();
else
SDA_LOW();
IIC_Delay();
SCL_HIGH();
IIC_Delay();
SCL_LOW();
IIC_Delay();
data <<= 1;
}
}

uint8_t IIC_ReadAck(void)
{
uint8_t ack;
SDA_HIGH(); 
IIC_Delay();
ack = SDA_READ();
IIC_Delay();
SCL_HIGH();

SCL_LOW();
IIC_Delay();
return ack;
}

void IIC_SendData(uint8_t dat)
{
IIC_Start();
IIC_SendByte(dat);
IIC_ReadAck();
IIC_Stop();
}
