#include "user_check_button.h"

uint16_t BT_enter=0, BT_esc=0, BT_up=0, BT_down=0;

uint32_t stampTime1=0;
uint32_t stampTime2=0;
uint32_t stampTime3=0;
uint32_t *ptrStamp;

uint32_t startpage = FLASH_USER_START_ADDR;
uint32_t check=1;

void Check_BT_ENTER(uint16_t *State,uint16_t *checkState, uint16_t *setupCount,uint32_t *time1, uint32_t *time2, uint32_t *time3)
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
			*checkState=1;
			*time1=stampTime1;
			*time2=stampTime2;
			*time3=stampTime3;
			FLASH_WritePage(FLASH_USER_START_ADDR, FLASH_USER_END_ADDR, check, *time1, *time2, *time3);
		}
		else
		{
			stampTime1=*time1;
			stampTime2=*time2;
			stampTime3=*time3;
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

void BT_Esc_Exit_Setup(uint16_t *State, uint16_t *setupCount,uint32_t *time1, uint32_t *time2, uint32_t *time3)
{
	BT_Press_Hold_Esc(GPIOB, GPIO_PIN_5, State, BT_up, BT_down);
	if(*State==1)
	{
		stampTime1=*time1;
		stampTime2=*time2;
		stampTime3=*time3;
		*setupCount=0;
	}
}


void FLASH_WritePage(uint32_t startPage, uint32_t endPage,uint32_t check, uint32_t data1,uint32_t data2,uint32_t data3)
{
  HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = startPage;
	EraseInit.NbPages = (endPage - startPage)/FLASH_PAGE_SIZE;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage , check);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage + 4, data1); //4 byte dau tien
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage + 8, data2); // 4byte tiep theo
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage + 12, data3); // 4byte tiep theo
  HAL_FLASH_Lock();
}


uint32_t FLASH_ReadData32(uint32_t addr)
{
	uint32_t data = *(__IO uint32_t *)(addr);
	return data;
}

void Run_Begin(uint16_t *setupCount, uint32_t time1, uint32_t time2,uint32_t time3)
{
	stampTime1=time1;
	stampTime2=time2;
	stampTime3=time3;
	*setupCount=1;
	ptrStamp=&stampTime1;
	LCD_Change_State_Setup_T1_T2_T3(stampTime1, stampTime2, stampTime3);
	
	UintTime_To_CharTime_T1_T2_T3(stampTime1, stampTime2, stampTime3);
}

