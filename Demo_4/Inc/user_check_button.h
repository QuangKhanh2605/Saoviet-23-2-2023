#ifndef __USER_CHECK_BUTTON_
#define __USER_CHECK_BUTTON_

#include "main.h"
#include "user_LCD_object.h"


void Check_BT_ENTER(uint16_t *State,uint16_t *checkState, uint16_t *setupCount,uint16_t time1, uint16_t time2, uint16_t time3);
void Check_BT_ESC(uint16_t State, uint16_t *setupCount);
void Check_BT_UP(uint16_t State);
void Check_BT_DOWN(uint16_t State);


#endif
