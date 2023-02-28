#include "main.h"
#include "CLCD.h"
#include <stdio.h>
#include "user_LCD.h"
#include "check_Button.h"

#define RELAY1 GPIO_PIN_2 
#define RELAY2 GPIO_PIN_3
#define RELAY3 GPIO_PIN_0
#define RELAY4 GPIO_PIN_1
#define LED5 	 GPIO_PIN_0
#define LED6	 GPIO_PIN_1

TIM_HandleTypeDef htim2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void delay_1ms(void);
void delay_s(int time);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

uint32_t runTime=0;
uint16_t time1=1; //so giay cho an
uint16_t time2=1;	//so phut giua 2 chu ky ban
uint16_t time3=1; //thoi gian giua 2 moto vang va chinh luong 

uint16_t setupCount;
uint16_t stampTime1;
uint16_t stampTime2;
uint16_t stampTime3;
uint16_t *ptrStamp;

uint16_t State;
uint16_t countState;

//gio phut giay
uint16_t hh, mm, ss;

uint16_t BT_up, BT_down;

CLCD_Name LCD;

void set(uint32_t *runTime, GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
											      GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  
                            GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3);

void reset(uint32_t *runTime, GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                              GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  
                              GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3);

void check_test(void);
void set_time(uint16_t *hh, uint16_t *mm, uint16_t *ss);

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
	setupCount=1;
	runTime=0;
	countState=0;
	State=1;
	
	while (State==1) 
	{
		CLCD_SetCursor(&LCD,3,0);
		CLCD_WriteString(&LCD,"Nhan ENTER");
	}
	
  while (1)
  {	
		set_time(&hh, &mm, &ss);
		
		if(countState==0)
		{
			set(&runTime, GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		}
		if(countState==1 && runTime>=2 )
		{
			set(&runTime, GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		}
		if(countState==2 && runTime>=time1 )
		{
			reset(&runTime, GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		}
		if(countState==3 && runTime>=time3 )
		{
			reset(&runTime, GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		}
		if(countState==4 && runTime>=60*time2)
		{
			runTime=0;
			countState=0;
		}
		
		if(State==1) 
		{
			LCD_running_X1(&LCD, hh, mm, ss);
			LCD_running_X2(&LCD, time1, time2, time3);
		}
		else 
		{
			press_click_BT_up(&BT_up, ptrStamp);
			press_click_BT_down(&BT_down, ptrStamp);
			
			press_hold_BT_up(GPIOB, GPIO_PIN_3, ptrStamp);
			press_hold_BT_down(GPIOB, GPIO_PIN_4, ptrStamp);
			
			LCD_setup_X1(&LCD, hh, mm, ss, setupCount);
			LCD_setup_X2(&LCD, *ptrStamp, setupCount);
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_2)
	{
		if (State==0) State=1;
		else 				  State=0;
		
		if(State==1)
		{
			check_test();
			runTime=0;
			countState=0;
			
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
		setupCount=1;
		ptrStamp=&stampTime1;
	}
	
	if(GPIO_Pin == GPIO_PIN_5 )
	{
		if(State==0)
		{
		if(setupCount==3) setupCount=1;
		else 						  setupCount++;
		if(setupCount==1 ) ptrStamp=&stampTime1;
		if(setupCount==2 ) ptrStamp=&stampTime2;
		if(setupCount==3 ) ptrStamp=&stampTime3;
		}
	}
	
	if(GPIO_Pin == GPIO_PIN_4 )
	{
		if(State==0) 
		{
			BT_down=1;
		}
	}
	
	if(GPIO_Pin == GPIO_PIN_3)
	{
		if(State==0) 
		{
			BT_up=1;
		}
	}
}

void set(uint32_t *runTime, GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                            GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  
                            GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3)
{
	HAL_GPIO_WritePin(GPIO1, GPIO_Pin1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO2, GPIO_Pin2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO3, GPIO_Pin3,GPIO_PIN_SET);
		countState++;
		*runTime=0;
}
void reset(uint32_t *runTime, GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                              GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  
                              GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3)
{
	HAL_GPIO_WritePin(GPIO1, GPIO_Pin1,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO2, GPIO_Pin2,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO3, GPIO_Pin3,GPIO_PIN_RESET);
		countState++;
		*runTime=0;
}

void set_time(uint16_t *hh, uint16_t *mm, uint16_t *ss)
{
	const uint16_t MINUTES_OF_THE_HOUR=60;
	const uint16_t SECOND_OF_THE_HOUR=60;
	*hh=runTime/(MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR);
	*mm=(runTime-(*hh)*MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR)/SECOND_OF_THE_HOUR;
	*ss=(runTime-(*hh)*MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR-(*mm)*SECOND_OF_THE_HOUR);
	if(runTime>=86399)
	{
	runTime=0; *hh=0; *mm=0; *ss=0;
	}
}

void check_test(void)
{
	if(State == 0)
	{
		reset(&runTime, GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		reset(&runTime, GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		countState=0;
		runTime=0;
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
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins :  PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PB3 PB4*/
	GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn,1, 1);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

void Error_Handler(void)
{
 
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  UNUSED(htim);
	runTime++;
}
void delay_1ms(void)
{
	__HAL_TIM_SetCounter(&htim2,0);
	while (__HAL_TIM_GetCounter(&htim2)<1000);
}
void delay_s(int time)
{
	int i=0;
	for(i=0;i<time;i++)
	{
		delay_1ms();
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

