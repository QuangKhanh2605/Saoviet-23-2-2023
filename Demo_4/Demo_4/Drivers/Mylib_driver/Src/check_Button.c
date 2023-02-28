#include "check_Button.h"

uint16_t check_BT_run_up;
uint16_t check_BT_run_down;

uint16_t run_BT_time;
uint16_t run_BT_begin;

void press_click_BT_up(uint16_t *BT_up, uint16_t *ptr_stamp)
{
	if(*BT_up == 1)
	{
		(*ptr_stamp)++;
		 *BT_up=0;
	}
}

void press_click_BT_down(uint16_t *BT_down, uint16_t *ptr_stamp)
{
	if(*BT_down == 1)
	{
		if(*ptr_stamp > 0)(*ptr_stamp)--;
		 *BT_down=0;
	}
}

void press_hold_BT_up( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0) 
	{
		check_BT_run_up=1;
		if (run_BT_time>400)
		{
			run_BT_time=0;
			(*ptr_stamp)++;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_Pinx)==1) 
	{
		check_BT_run_up=0;
	}
}

void press_hold_BT_down( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0) 
	{
		check_BT_run_down=1;
		if (run_BT_time>400 && (*ptr_stamp)>0)
		{
			run_BT_time=0;
			(*ptr_stamp)--;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_Pinx)==1) 
	{
		check_BT_run_down=0;
	}
}
	
void HAL_SYSTICK_Callback(void)
{
	if(check_BT_run_up == 1 || check_BT_run_down == 1) run_BT_begin++;
	else 
	{
		run_BT_begin=0;
		run_BT_time=0;
	}
	if(run_BT_begin>500) run_BT_time++;
}
