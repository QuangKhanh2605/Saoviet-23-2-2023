#include "check_Button.h"

uint16_t check_BT_Run_Up;
uint16_t check_BT_Run_Down;

uint16_t run_BT_Time;
uint16_t run_BT_Begin;

void press_Click_BT_Up(uint16_t *BT_UP, uint16_t *ptr_stamp)
{
	if(*BT_UP == 1)
	{
		(*ptr_stamp)++;
		 *BT_UP=0;
	}
}

void press_Click_BT_Down(uint16_t *BT_Down, uint16_t *ptr_stamp)
{
	if(*BT_Down == 1)
	{
		if(*ptr_stamp > 0)(*ptr_stamp)--;
		 *BT_Down=0;
	}
}

void press_Hold_BT_Up( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0) 
	{
		check_BT_Run_Up=1;
		if (run_BT_Time>400)
		{
			run_BT_Time=0;
			(*ptr_stamp)++;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_Pinx)==1) 
	{
		check_BT_Run_Up=0;
	}
}

void press_Hold_BT_DOWN( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0) 
	{
		check_BT_Run_Down=1;
		if (run_BT_Time>400 && (*ptr_stamp)>0)
		{
			run_BT_Time=0;
			(*ptr_stamp)--;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_Pinx)==1) 
	{
		check_BT_Run_Down=0;
	}
}
	
void HAL_SYSTICK_Callback(void)
{
	if(check_BT_Run_Up == 1 || check_BT_Run_Down == 1) run_BT_Begin++;
	else 
	{
		run_BT_Begin=0;
		run_BT_Time=0;
	}
	if(run_BT_Begin>500) run_BT_Time++;
}
