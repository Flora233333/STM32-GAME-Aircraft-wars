#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void);
uint8_t Read_Key(void);

#endif
