#include "user_LCD.h"
#include "stdio.h"
//CLCD_Name LCD;


void LCD_running_X1(CLCD_Name* LCD, uint16_t hh, uint16_t mm, uint16_t ss)
{
	CLCD_SetCursor(LCD,0,0);
	CLCD_WriteString(LCD,"Running ");
	char h[3],m[3],s[3];
	
	if(hh<10) sprintf(h,"0%d:", hh);
	else sprintf(h,"%d:", hh);
	CLCD_SetCursor(LCD,8,0);
	CLCD_WriteString(LCD,h);
	
	if(mm<10) sprintf(m,"0%d:", mm);
	else sprintf(m,"%d:", mm);
	CLCD_SetCursor(LCD,11,0);
	CLCD_WriteString(LCD,m);
	
	if(ss<10) sprintf(s,"0%d", ss);
	else sprintf(s,"%d", ss);
	CLCD_SetCursor(LCD,14,0);
	CLCD_WriteString(LCD,s);
}
void LCD_running_X2(CLCD_Name* LCD, uint16_t t1, uint16_t t2, uint16_t t3)
{
	char LCD_send[16];
	sprintf(LCD_send,"T1:%d T2:%d T3:%d     ", t1,t2,t3);
	CLCD_SetCursor(LCD,0,1);
	CLCD_WriteString(LCD,LCD_send);
}

void LCD_setup_X1(CLCD_Name* LCD, uint16_t hh, uint16_t mm, uint16_t ss, uint16_t setupCount)
{
	CLCD_SetCursor(LCD,0,0);
	if(setupCount==1) 			CLCD_WriteString(LCD,"SET_T1 ");
	else if(setupCount==2) CLCD_WriteString(LCD,"SET_T2 ");
	else			  	CLCD_WriteString(LCD,"SET_T3 ");
	
	char h[3],m[3],s[3];
	
	if(hh<10) sprintf(h,"0%d:", hh);
	else sprintf(h,"%d:", hh);
	CLCD_SetCursor(LCD,8,0);
	CLCD_WriteString(LCD,h);
	
	if(mm<10) sprintf(m,"0%d:", mm);
	else sprintf(m,"%d:",mm);
	CLCD_SetCursor(LCD,11,0);
	CLCD_WriteString(LCD,m);
	
	if(ss<10) sprintf(s,"0%d", ss);
	else sprintf(s,"%d", ss);
	CLCD_SetCursor(LCD,14,0);
	CLCD_WriteString(LCD,s);
}
void LCD_setup_X2(CLCD_Name* LCD, uint16_t time, uint16_t setupCount)
{
	char LCD_send[16];
	if(setupCount==1) 			sprintf(LCD_send,"T1:%d Giay       ", time);
	else if(setupCount==2) sprintf(LCD_send,"T2:%d Phut       ", time);
	if(setupCount==3) 			sprintf(LCD_send,"T3:%d Giay       ", time);	
	
	CLCD_SetCursor(LCD,0,1);
	CLCD_WriteString(LCD,LCD_send);
}



