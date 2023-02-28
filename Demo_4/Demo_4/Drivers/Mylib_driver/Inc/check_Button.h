#ifndef __CHECK_BUTTON_H
#define __CHECK_BUTTON_H

#include "stm32l1xx_it.h"
#include <stdio.h>
#include "stm32l1xx_hal.h"

void HAL_SYSTICK_Callback(void);

void press_click_BT_up(uint16_t *BT_up, uint16_t *ptr_stamp);
void press_click_BT_down(uint16_t *BT_down, uint16_t *ptr_stamp);

void press_hold_BT_up( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp);
void press_hold_BT_down( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp);


#endif
