#include "check_Button.h"

uint16_t check_BT_run_up;
uint16_t check_BT_run_down;

uint16_t run_BT_time;
uint16_t run_BT_begin;

const uint16_t run_BT_end=500;
const uint16_t run_BT_irq=100;

void BT_Press_Click_Up(uint16_t *BT_up, uint16_t *ptr_stamp)
{
	if(*BT_up == 1)
	{
		(*ptr_stamp)++;
		 *BT_up=0;
	}
}

void BT_Press_Click_Down(uint16_t *BT_down, uint16_t *ptr_stamp)
{
	if(*BT_down == 1)
	{
		if(*ptr_stamp > 0)(*ptr_stamp)--;
		 *BT_down=0;
	}
}

void BT_Press_Hold_Up( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0) 
	{
		check_BT_run_up=1;
		if (run_BT_time>run_BT_irq)
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

void BT_Press_Hold_Down( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *ptr_stamp)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0) 
	{
		check_BT_run_down=1;
		if (run_BT_time>run_BT_irq && (*ptr_stamp)>0)
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
	if(check_BT_run_up == 1 || check_BT_run_down == 1) 
	{
		run_BT_begin++;
	}
	else 
	{
		run_BT_begin=0;
		run_BT_time=0;
	}
	
	if(run_BT_begin>run_BT_end) run_BT_time++;
}
