#include "SoftIIC.h"
// 延时一下
static void SoftIIC_Delay(void)
{
    for (volatile int cnt = 0; cnt < 200; cnt++);
}

#define IIC_SDA_HIGH() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
#define IIC_SDA_LOW()  HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
#define IIC_SCL_HIGH() HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET)
#define IIC_SCL_LOW()  HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)
#define IIC_SDA_READ() HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)

void SoftIIC_Start(void)
{
    IIC_SDA_HIGH();
    IIC_SCL_HIGH();
    SoftIIC_Delay();

    IIC_SDA_LOW(); 
    SoftIIC_Delay();

    IIC_SCL_LOW();
    SoftIIC_Delay();
}

void SoftIIC_Stop(void)
{
    IIC_SDA_LOW();
    SoftIIC_Delay();

    IIC_SCL_HIGH();
    SoftIIC_Delay();

    IIC_SDA_HIGH();
    SoftIIC_Delay();
}

void SoftIIC_SendByte(uint8_t byte)
{
    for (int bit = 0; bit < 8; bit++)
    {
        if (byte & 0x80) 
            IIC_SDA_HIGH();
        else
            IIC_SDA_LOW();

        SoftIIC_Delay();
        IIC_SCL_HIGH();
        SoftIIC_Delay();
        IIC_SCL_LOW();
        SoftIIC_Delay();

        byte <<= 1;
    }
}

uint8_t SoftIIC_WaitAck(void)
{
    uint8_t ack;

    IIC_SDA_HIGH();  
    SoftIIC_Delay();

    ack = IIC_SDA_READ(); 
    SoftIIC_Delay();

    IIC_SCL_HIGH();
    SoftIIC_Delay();
    IIC_SCL_LOW();
    SoftIIC_Delay();

    return ack; 
}

void SoftIIC_SendData(uint8_t data)
{
    SoftIIC_Start();
    SoftIIC_SendByte(data);
    SoftIIC_WaitAck();
    SoftIIC_Stop();
}
