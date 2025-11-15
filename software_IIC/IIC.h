#ifndef __SOFTIIC_H
#define __SOFTIIC_H

#include "main.h"
#include "stm32f1xx_hal.h"

void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(uint8_t data);
uint8_t IIC_ReadAck(void);
void IIC_SendData(uint8_t dat);

#endif
