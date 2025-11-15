#include "main.h"

TIM_HandleTypeDef htim3;  // PWM
TIM_HandleTypeDef htim2;  // Encoder

int speed = 0;            // 在调试模式看变量
int pwmValue = 400;       // 默认

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM3_Init();
    MX_TIM2_Init();
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwmValue); // 给一个 PWM

    while (1)
    {
        static uint32_t lastCnt = 0;
        uint32_t nowCnt = __HAL_TIM_GET_COUNTER(&htim2);

        speed = nowCnt - lastCnt;
        lastCnt = nowCnt;

        HAL_Delay(100);  // 缓
    }
}
