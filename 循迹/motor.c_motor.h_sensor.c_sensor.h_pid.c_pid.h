Sensor.c:
#include "sensor.h"

void Sensor_Init(void)
{

}

void Sensor_Read(uint8_t *buf)
{
buf[0] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
buf[1] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
buf[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
buf[3] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
buf[4] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
buf[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
buf[6] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
buf[7] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
}


static uint8_t buf_to_mask(const uint8_t *buf)
{
uint8_t m = 0;

for(int i=0;i<8;i++){
if(buf[i]) m |= (1 << i);
}
return m;
}

float Sensor_GetPosition(void)
{
uint8_t buf[8];
Sensor_Read(buf);
uint8_t m = buf_to_mask(buf);

const float weights[8] = {-3.5f, -2.5f, -1.5f, -0.5f, 0.5f, 1.5f, 2.5f, 3.5f};

float sum = 0.0f;
int cnt = 0;
for(int i=0;i<8;i++){
if(m & (1 << i)){
sum += weights[i];
cnt++;
}
}

if(cnt == 0){

return 100.0f;
}

return sum / (float)cnt;
}

Sensor.h:
#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "stm32f1xx_hal.h"

void Sensor_Init(void);
void Sensor_Read(uint8_t *buf); 
float Sensor_GetPosition(void); 
#endif

Motor.c:
// Core/Src/motor.c
#include "motor.h"
#include "main.h"

#define PWM_MAX (htim2.Init.Period) // 假定 TIM2 用作 PWM

extern TIM_HandleTypeDef htim2; 


#define AIN1_PIN GPIO_PIN_2
#define AIN1_PORT GPIOA
#define AIN2_PIN GPIO_PIN_3
#define AIN2_PORT GPIOA
#define BIN1_PIN GPIO_PIN_4
#define BIN1_PORT GPIOA
#define BIN2_PIN GPIO_PIN_5
#define BIN2_PORT GPIOA
#define STBY_PIN GPIO_PIN_12
#define STBY_PORT GPIOB

void motor_init(void){HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PA0 左
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // PA1 右

HAL_GPIO_WritePin(STBY_PORT, STBY_PIN, GPIO_PIN_SET);
}

void motor_set_pwm(Motor_t m, uint16_t pwm){
if(pwm > PWM_MAX) pwm = PWM_MAX;
if(m==MOTOR_LEFT){
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm);
} else {
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm);
}
}

void motor_set_dir(Motor_t m, uint8_t dir){
// dir 0 正转，1 反转
if(m==MOTOR_LEFT){
HAL_GPIO_WritePin(AIN1_PORT, AIN1_PIN, dir?GPIO_PIN_SET:GPIO_PIN_RESET);
HAL_GPIO_WritePin(AIN2_PORT, AIN2_PIN, dir?GPIO_PIN_RESET:GPIO_PIN_SET);
} else {
HAL_GPIO_WritePin(BIN1_PORT, BIN1_PIN, dir?GPIO_PIN_SET:GPIO_PIN_RESET);
HAL_GPIO_WritePin(BIN2_PORT, BIN2_PIN, dir?GPIO_PIN_RESET:GPIO_PIN_SET);
}
}

Motor.h:
// Core/Inc/motor.h
#ifndef MOTOR_H
#define MOTOR_H
#include "stm32f1xx_hal.h"
typedef enum {MOTOR_LEFT=0, MOTOR_RIGHT=1} Motor_t;
void motor_init(void);
void motor_set_pwm(Motor_t m, uint16_t pwm);
void motor_set_dir(Motor_t m, uint8_t dir); 
#endif

Pid.c:
// Core/Src/pid.c
#include "pid.h"
#include <math.h>
void pid_init(PID_t *p, float Kp, float Ki, float Kd){
p->Kp=Kp; p->Ki=Ki; p->Kd=Kd; p->integr=0; p->prev=0;
p->out_min = -1e6f; p->out_max = 1e6f;
}
float pid_update(PID_t *p, float target, float measure, float dt){
float err = target - measure;
p->integr += err * dt;
float deriv = (err - p->prev) / dt;
float out = p->Kp*err + p->Ki*p->integr + p->Kd*deriv;
if(out > p->out_max) out = p->out_max;
if(out < p->out_min) out = p->out_min;
p->prev = err;
return out;
}

Pid.h:
// Core/Inc/pid.h
#ifndef PID_H
#define PID_H
typedef struct { float Kp, Ki, Kd; float integr, prev; float out_min, out_max; } PID_t;
void pid_init(PID_t *p, float Kp, float Ki, float Kd);
float pid_update(PID_t *p, float target, float measure, float dt);
#endif

Encoder.c:
// Core/Src/encoder.c
#include "encoder.h"
#include "main.h"

extern TIM_HandleTypeDef htim3; // left encoder (PA6/PA7)
extern TIM_HandleTypeDef htim4; // right encoder (PB6/PB7)

void encoder_init(void){
HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

int32_t encoder_get_left(void){

return (int32_t)__HAL_TIM_GET_COUNTER(&htim3);
}
int32_t encoder_get_right(void){
return (int32_t)__HAL_TIM_GET_COUNTER(&htim4);
}
void encoder_clear(void){
__HAL_TIM_SET_COUNTER(&htim3, 0);
__HAL_TIM_SET_COUNTER(&htim4, 0);
}

Encoder.h:
// Core/Inc/encoder.h
#ifndef ENCODER_H
#define ENCODER_H
#include "stm32f1xx_hal.h"
void encoder_init(void);
int32_t encoder_get_left(void);
int32_t encoder_get_right(void);
void encoder_clear(void);
#endif
