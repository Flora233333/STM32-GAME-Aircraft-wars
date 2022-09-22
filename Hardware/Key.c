#include "Key.h"                  // Device header

void Key_Init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t Read_Key(void) {
	uint8_t flag = 0;
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1) {
		
		flag = 1;
	}
    
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1) {
		
		flag = 2;
	}
    
	return flag;
}
