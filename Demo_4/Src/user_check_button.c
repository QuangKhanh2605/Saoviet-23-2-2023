#include "user_check_button.h"

uint16_t BT_enter=0, BT_esc=0, BT_up=0, BT_down=0;


uint16_t stampTime1;
uint16_t stampTime2;
uint16_t stampTime3;
uint16_t *ptrStamp;

void Check_BT_ENTER(uint16_t *State,uint16_t *checkState, uint16_t *setupCount, uint16_t time1, uint16_t time2, uint16_t time3)
{
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2)==0)
	{
		BT_enter=1;
	}
	if(BT_enter==1 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2)==1)
	{
		BT_enter=0;
		
		if (*State==0) *State=1;
		else 				  *State=0;
		
		if(*State==1)
		{
//			Check_Test();
			*checkState=1;
			time1=stampTime1;
			time2=stampTime2;
			time3=stampTime3;
		}
		else
		{
			stampTime1=time1;
			stampTime2=time2;
			stampTime3=time3;
		}
		*setupCount=1;
		ptrStamp=&stampTime1;
	}
	
}

void Check_BT_ESC(uint16_t State, uint16_t *setupCount)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)==0)
	{
		BT_esc=1;
	}
	if(BT_esc==1 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)==1)
	{
		USER_LCD_Change_Setup();
		BT_esc=0;
		if(*setupCount==3) *setupCount=1;
		else 						   (*setupCount)++;
		if(*setupCount==1 ) ptrStamp=&stampTime1;
		if(*setupCount==2 ) ptrStamp=&stampTime2;
		if(*setupCount==3 ) ptrStamp=&stampTime3;
	}
}
void Check_BT_UP(uint16_t State)
{
	if (State==0)
	{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==0 && BT_up==0)
	{
		BT_up=1;
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==1)
	{
		BT_up=0;
	}
	}
}
void Check_BT_DOWN(uint16_t State)
{
	if (State==0)
	{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==0 && BT_down==0)
	{
		BT_down=1;
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==1)
	{
		BT_down=0;
	}
	}
}

void BT_Check_Up_Down(void)
{
	BT_Press_Click_Up(&BT_up, ptrStamp);
	BT_Press_Click_Down(&BT_down, ptrStamp);
			
	BT_Press_Hold_Up(GPIOB, GPIO_PIN_3, ptrStamp);
	BT_Press_Hold_Down(GPIOB, GPIO_PIN_4, ptrStamp);
	
	LCD_Change_State_Setup_T1_T2_T3(stampTime1, stampTime2, stampTime3);
	
	UintTime_To_CharTime_T1_T2_T3(stampTime1, stampTime2, stampTime3);
	
}

