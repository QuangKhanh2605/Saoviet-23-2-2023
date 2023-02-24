#ifndef __CHECK_BUTTON_H
#define __CHECK_BUTTON_H

#include "stm32l1xx_it.h"
#include <stdio.h>
#include "stm32l1xx_hal.h"

void press_Click_BT( uint16_t *state, uint16_t *BT_Enter, uint16_t *BT_Down, uint16_t *BT_UP, uint16_t *BT_Esc);
void press_Hold_BT( GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2);

#endif