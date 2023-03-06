#ifndef __USER_CHECK_BUTTON_
#define __USER_CHECK_BUTTON_

#include "main.h"
#include "user_LCD_object.h"

#define FLASH_ADDR_PAGE_126 ((uint32_t)0x0801F810)	//Page 126
#define FLASH_ADDR_PAGE_127 ((uint32_t)0x0801FC00)	//Page 127

#define FLASH_USER_START_ADDR   FLASH_ADDR_PAGE_126
#define FLASH_USER_END_ADDR     FLASH_ADDR_PAGE_127 + FLASH_PAGE_SIZE

void Check_BT_ENTER(uint16_t *State,uint16_t *checkState, uint16_t *setupCount,uint32_t *time1, uint32_t *time2, uint32_t *time3);
void Check_BT_ESC(uint16_t State, uint16_t *setupCount);
void Check_BT_UP(uint16_t State);
void Check_BT_DOWN(uint16_t State);

uint32_t FLASH_ReadData32(uint32_t addr);
void FLASH_WritePage(uint32_t startPage, uint32_t endPage,uint32_t check, uint32_t data1,uint32_t data2,uint32_t data3);

#endif
