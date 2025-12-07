
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim == &htim2){
        HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
    }
}


void Delay_us(uint16_t us){
    uint16_t i;
    for(i=0; i<us; i++){
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY2_Pin)
    {
        HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
    }
}


HAL_TIM_Base_Start_IT(&htim2);

__HAL_TIM_SET_AUTORELOAD(&htim2, 19999);
uint8_t i = 0;  

HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);

while (1)
{
    
    if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
    {
        Delay_us(50);
        if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
        {
      
            while (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
            {
                Delay_us(10);
            }
            i = (i + 1) % 3;
          
            switch(i)
            {
                case 0:  // 慢
                    __HAL_TIM_SET_AUTORELOAD(&htim2, 19999);
                    break;
                case 1:  // 中 
                    __HAL_TIM_SET_AUTORELOAD(&htim2, 9999);
                    break;
                case 2:  // 快
                    __HAL_TIM_SET_AUTORELOAD(&htim2, 3999);
                    break;
                default: // 异常情况默认
                    __HAL_TIM_SET_AUTORELOAD(&htim2, 19999);
                    break;
            }    
            HAL_TIM_Base_Stop(&htim2);    // 重启定时器
            HAL_TIM_Base_Start(&htim2);
        }
    }
    
    Delay_us(100);  
