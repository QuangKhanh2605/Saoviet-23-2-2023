#ifndef __USER_LCD_H
#define	__USER_LCD_H
	
#include <stdio.h>
#include "CLCD.h"

void LCD_running_X1(CLCD_Name* LCD, uint16_t hh, uint16_t mm, uint16_t ss);
void LCD_running_X2(CLCD_Name* LCD, uint16_t time1, uint16_t time2, uint16_t time3);

void LCD_setup_X1(CLCD_Name* LCD, uint16_t hh, uint16_t mm, uint16_t ss, uint16_t setupCount);
void LCD_setup_X2(CLCD_Name* LCD, uint16_t time, uint16_t setupCount);

#endif
