#include "main.h"
#include "CLCD.h"
#include "user_LCD.h"
#include "check_Button.h"
#include "Relay_Led.h"
#include "user_LCD_object.h"
#include "user_check_button.h"

#define RELAY1 GPIO_PIN_2 
#define RELAY2 GPIO_PIN_3
#define RELAY3 GPIO_PIN_0
#define RELAY4 GPIO_PIN_1
#define LED5   GPIO_PIN_0
#define LED6   GPIO_PIN_1

uint32_t runTime=0;
uint16_t time1=1; //so giay cho an
uint16_t time2=2;	//so phut giua 2 chu ky ban
uint16_t time3=1; //thoi gian giua 2 moto vang va chinh luong 

uint16_t State=1;
uint16_t checkState=0;
uint16_t countState=0;
uint16_t setupCount=1;

//gio phut giay
uint16_t hh, mm, ss;

CLCD_Name LCD;

TIM_HandleTypeDef htim2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void Check_BT_Callback(void);

void Delay_1ms(void);
void Delay_s(int time);
void Check_Test(void);
void Set_Time(uint16_t *hh, uint16_t *mm, uint16_t *ss);
void Run_Feed_Shrimp(void);
void BT_Check_Up_Down(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
  CLCD_4BIT_Init(&LCD, 16,2, GPIOC, GPIO_PIN_8,GPIOC, GPIO_PIN_7,
                             GPIOC, GPIO_PIN_6,GPIOB, GPIO_PIN_15,
                             GPIOB, GPIO_PIN_14,GPIOB, GPIO_PIN_12);

	while (State==1) 
	{
		CLCD_SetCursor(&LCD,3,0);
		CLCD_WriteString(&LCD,"Nhan ENTER");
		Check_BT_ENTER(&State, &checkState, &setupCount, time1, time2, time3);
	}
	
	CLCD_SetCursor(&LCD, 8,0);
	CLCD_WriteString(&LCD, "00:00:00");
	
	while (State==0)
		{
			USER_LCD_Display_Running_OR_Setup(State);
			BT_Check_Up_Down();
			USER_LCD_Display_Setup(&LCD, setupCount);
			Check_BT_Callback();
		}
	
  while (1)
  {	
		Set_Time(&hh, &mm, &ss);
		Run_Feed_Shrimp();
		
		LCD_Change_State_Time_HH_MM_SS(hh,mm,ss);
		UintTime_To_CharTime_HH_MM_SS(hh,  mm, ss);
		USER_LCD_Display_Time(&LCD);
		
		Check_BT_Callback();
		
		if(State==0 )
		{
			USER_LCD_Display_Running_OR_Setup(State);
			BT_Check_Up_Down();
			USER_LCD_Display_Setup(&LCD, setupCount);
		}
		
		if(State==1) 
		{
			USER_LCD_Display_Running_OR_Setup(State);
			USER_LCD_Display_Running(&LCD, setupCount);
			Check_Test();
		}
	}
}

void Run_Feed_Shrimp(void)
{
	if(countState==0)
	{
		Set_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		countState++;
		runTime=0;
	}
	if(countState==1 && runTime>=2 )
	{
		Set_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		countState++;
		runTime=0;
	}
	if(countState==2 && runTime>=time1 )
	{
		Reset_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		countState++;
		runTime=0;
	}
	if(countState==3 && runTime>=time3 )
	{
		Reset_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		countState++;
		runTime=0;
	}
	if(countState==4 && runTime>=60*time2)
	{
		runTime=0;
		countState=0;
	}
}

void Check_BT_Callback(void)
{
	Check_BT_ENTER(&State, &checkState, &setupCount, time1, time2, time3);
	Check_BT_ESC(State, &setupCount);
	Check_BT_UP(State);
	Check_BT_DOWN(State);
		
	Check_Test();
}

void Set_Time(uint16_t *hh, uint16_t *mm, uint16_t *ss)
{
	const uint16_t MINUTES_OF_THE_HOUR=60;
	const uint16_t SECOND_OF_THE_HOUR=60;
	const uint32_t SECOND_OF_THE_DAY=86400;
	
	*hh=runTime/(MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR);
	*mm=(runTime-(*hh)*MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR)/SECOND_OF_THE_HOUR;
	*ss=(runTime-(*hh)*MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR-(*mm)*SECOND_OF_THE_HOUR);
	
	if(runTime>=SECOND_OF_THE_DAY-1)
	{
	runTime=0; *hh=0; *mm=0; *ss=0;
	}
}

void Check_Test(void)
{
	if(checkState==1)
	{
		Reset_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		Reset_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		countState=0;
		runTime=0;
		checkState=0;
	}
	
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2 
                          |GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_11|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC0 PC1 PC2 
                           PC3 PC6 PC7 PC8 
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2 
                          |GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA11 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_11|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void Error_Handler(void)
{
 
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  UNUSED(htim);
	runTime++;
}
void Delay_1ms(void)
{
	__HAL_TIM_SetCounter(&htim2,0);
	while (__HAL_TIM_GetCounter(&htim2)<1000);
}
void Delay_s(int time)
{
	int i=0;
	for(i=0;i<time;i++)
	{
		Delay_1ms();
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

