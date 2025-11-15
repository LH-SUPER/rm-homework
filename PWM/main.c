//其实没有实操，理论学完之后debugger的error为0就没管了（因为买的江协套件有外设缺失
int speed = 0;
int lastCount = 0;

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM3_Init(); // PWM
    MX_TIM2_Init(); // Encoder

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 400);

    while (1)
    {
        int now = __HAL_TIM_GET_COUNTER(&htim2);
        speed = now - lastCount; // 速度
        lastCount = now;

        HAL_Delay(100);
    }
}
