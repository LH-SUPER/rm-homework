//其实没有实操，理论学完之后debugger的error为0就没管了（因为买的江协套件有外设缺失
int speed = 0;
int pwmValue = 400;

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM3_Init();
    MX_TIM2_Init();

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwmValue);

    while (1)
    {
        static int last = 0;
        int now = __HAL_TIM_GET_COUNTER(&htim2);

        speed = now - last;
        last = now;

        HAL_Delay(100);
    }
}
