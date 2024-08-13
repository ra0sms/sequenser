#include "debug.h"

#define LED_Pin GPIO_Pin_7
#define LED_GPIO_Port GPIOC
//**************
#define OUT_1 GPIO_Pin_2
#define OUT_1_Port GPIOD
//***************
#define OUT_2 GPIO_Pin_3
#define OUT_2_Port GPIOD
//***************
#define OUT_3 GPIO_Pin_4
#define OUT_3_Port GPIOD
//***************
#define OUT_PLUS GPIO_Pin_5
#define OUT_PLUS_Port GPIOD
//***************
#define MODE GPIO_Pin_1
#define MODE_Port GPIOC
//***************
#define PTT_IN GPIO_Pin_0
#define PTT_IN_Port GPIOC
//***************
#define DELAY GPIO_Pin_2
#define DELAY_Port GPIOC
//***************
#define MAX_DELAY 45
#define MIN_DELAY 15


vu8 delay_ms = 0;

void Timer2_INIT(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIMBase_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIMBase_InitStruct.TIM_Period = arr;
    TIMBase_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIMBase_InitStruct.TIM_Prescaler = psc;
    TIMBase_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIMBase_InitStruct);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    TIM_Cmd(TIM2, ENABLE);
}

void GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = OUT_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OUT_1_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = OUT_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OUT_2_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = OUT_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OUT_3_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = OUT_PLUS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OUT_PLUS_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PTT_IN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PTT_IN_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MODE;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MODE_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DELAY;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DELAY_Port, &GPIO_InitStructure);

    GPIO_WriteBit(OUT_1_Port, OUT_1, Bit_RESET);
    GPIO_WriteBit(OUT_2_Port, OUT_2, Bit_RESET);
    GPIO_WriteBit(OUT_3_Port, OUT_3, Bit_RESET);
    GPIO_WriteBit(OUT_PLUS_Port, OUT_PLUS, Bit_RESET);

}

void bias_mode(){
    vu8 flag_on = 0;
    while (!GPIO_ReadInputDataBit(PTT_IN_Port, PTT_IN)){
        flag_on = 1;
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_1_Port, OUT_1, Bit_SET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_2_Port, OUT_2, Bit_SET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_3_Port, OUT_3, Bit_SET);
        GPIO_WriteBit(OUT_PLUS_Port, OUT_PLUS, Bit_SET);
    }
    if (flag_on){
        flag_on = 0;
        GPIO_WriteBit(OUT_PLUS_Port, OUT_PLUS, Bit_RESET);
        GPIO_WriteBit(OUT_3_Port, OUT_3, Bit_RESET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_2_Port, OUT_2, Bit_RESET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_1_Port, OUT_1, Bit_RESET);
    }
}

void LNA_mode(){
    vu8 flag_on = 0;
    while (!GPIO_ReadInputDataBit(PTT_IN_Port, PTT_IN)){
        flag_on = 1;
        GPIO_WriteBit(OUT_PLUS_Port, OUT_PLUS, Bit_RESET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_1_Port, OUT_1, Bit_SET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_2_Port, OUT_2, Bit_SET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_3_Port, OUT_3, Bit_SET);
    }
    if (flag_on){
        flag_on = 0;
        GPIO_WriteBit(OUT_3_Port, OUT_3, Bit_RESET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_2_Port, OUT_2, Bit_RESET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_1_Port, OUT_1, Bit_RESET);
        Delay_Ms(delay_ms);
        GPIO_WriteBit(OUT_PLUS_Port, OUT_PLUS, Bit_SET);
    }
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    GPIO_INIT();
    Timer2_INIT(999, 23999);
    while(1)
    {
        if (GPIO_ReadInputDataBit(DELAY_Port, DELAY)) delay_ms = MIN_DELAY;
        else delay_ms = MAX_DELAY;
        if (GPIO_ReadInputDataBit(MODE_Port, MODE)) bias_mode();
        else LNA_mode();
    }
}
