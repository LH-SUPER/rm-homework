#ifndef __SOFTIIC_H
#define __SOFTIIC_H

#include "main.h"
#include "stm32f1xx_hal.h"

void SoftIIC_Delay(void);
void SoftIIC_Start(void);
void SoftIIC_Stop(void);       
void SoftIIC_SendByte(uint8_t byte);
uint8_t SoftIIC_WaitAck(void); 
void SoftIIC_SendData(uint8_t data);
#endif
