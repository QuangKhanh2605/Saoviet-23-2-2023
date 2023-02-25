#ifndef __CHECK_BUTTON_H
#define __CHECK_BUTTON_H

#include "stm32l1xx_it.h"
#include <stdio.h>
#include "stm32l1xx_hal.h"

void HAL_SYSTICK_Callback(void);

void press_Click_BT_Up(uint16_t *BT_UP, uint16_t *ptr_stamp);
void press_Click_BT_Down(uint16_t *BT_Down, uint16_t *ptr_stamp);
void press_Hold_BT_Up( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp);
void press_Hold_BT_DOWN( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp);


#endif
