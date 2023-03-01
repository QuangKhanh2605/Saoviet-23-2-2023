#include "user_LCD.h"

const uint16_t ACSII_value_number=48;

void Variable_To_Char_Run_Time(char time[], uint16_t stamp)
{
	if(stamp<10)
	{
		time[0]='0';
		time[1]=stamp+ACSII_value_number;
	}
	else
	{
		time[0]=stamp/10+ACSII_value_number;
		time[1]=stamp%10+ACSII_value_number;
	}
}

void Variable_To_Char(char time[], uint16_t stamp, uint16_t *lengthStamp)
{
	uint16_t Division=10;
	while(stamp/Division>=1)
	{
		Division=Division*10;
		(*lengthStamp)++;
	}
		for(int j=*lengthStamp-1;j>=0;j--)
		{
		time[j]=stamp%10+ACSII_value_number;
		stamp=stamp/10;
		}
}

void LCD_Run_Time(char time[],char hh[], char mm[], char ss[])
{
	time[2]=time[5]=':';
	time[0]=hh[0]; time[1]=hh[1];
	time[3]=mm[0]; time[4]=mm[1];
	time[6]=ss[0]; time[7]=ss[1];
	time[8]=NULL;
}

void LCD_Running_X1(CLCD_Name* LCD, uint16_t hh, uint16_t mm, uint16_t ss)
{
	CLCD_SetCursor(LCD,0,0);
	CLCD_WriteString(LCD,"Running ");
	char h[3],m[3],s[3];
	char time[8];
	
	Variable_To_Char_Run_Time(h, hh);
	Variable_To_Char_Run_Time(m, mm);
	Variable_To_Char_Run_Time(s, ss);
	LCD_Run_Time(time, h, m, s);
	CLCD_SetCursor(LCD,8,0);
	CLCD_WriteString(LCD,time);
	
//	if(hh<10) sprintf(h,"0%d:", hh);
//	else sprintf(h,"%d:", hh);
//	CLCD_SetCursor(LCD,8,0);
//	CLCD_WriteString(LCD,h);
}

void LCD_Running_X2(CLCD_Name* LCD, uint16_t t1, uint16_t t2, uint16_t t3)
{
	char LCD_send[16];
	sprintf(LCD_send,"T1:%d T2:%d T3:%d     ", t1,t2,t3);
	CLCD_SetCursor(LCD,0,1);
	CLCD_WriteString(LCD,LCD_send);
}

void LCD_Setup_X1(CLCD_Name* LCD, uint16_t hh, uint16_t mm, uint16_t ss, uint16_t setupCount)
{
	CLCD_SetCursor(LCD,0,0);
	if(setupCount==1) 			CLCD_WriteString(LCD,"SET_T1 ");
	else if(setupCount==2) CLCD_WriteString(LCD,"SET_T2 ");
	else			  	CLCD_WriteString(LCD,"SET_T3 ");
	
	char h[3],m[3],s[3];
	char time[8];
	
	Variable_To_Char_Run_Time(h, hh);
	Variable_To_Char_Run_Time(m, mm);
	Variable_To_Char_Run_Time(s, ss);
	LCD_Run_Time(time, h, m, s);
	CLCD_SetCursor(LCD,8,0);
	CLCD_WriteString(LCD,time);
}

void LCD_Setup_X2(CLCD_Name* LCD, uint16_t time, uint16_t setupCount)
{
	char LCD_send[16];
	uint16_t lengthStamp=1;
	
	CLCD_SetCursor(LCD,0,1);
	if(setupCount==1)       CLCD_WriteString(LCD,"T1:");
	else if(setupCount==2)  CLCD_WriteString(LCD,"T1:");
	if(setupCount==3)       CLCD_WriteString(LCD,"T1:");	
	
	Variable_To_Char(LCD_send, time, &lengthStamp);
	CLCD_SetCursor(LCD,3,1);
	CLCD_WriteString(LCD,LCD_send);
	
	CLCD_SetCursor(LCD,3+lengthStamp,1);
	if(setupCount==1)       CLCD_WriteString(LCD," Giay           ");
	else if(setupCount==2)  CLCD_WriteString(LCD," Phut           ");
	if(setupCount==3)       CLCD_WriteString(LCD," Giay           ");	
}



