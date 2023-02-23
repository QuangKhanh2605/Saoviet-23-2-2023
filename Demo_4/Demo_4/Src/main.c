#include "main.h"
#include "CLCD.h"
#include <stdio.h>

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
//void SysTick_Handler(void);

int t=0;
int t1=1; //so giay cho an
int t2=1;	//so phut giua 2 chu ky ban
int t3=1; //thoi gian giua 2 moto vang va chinh luong 

int t01;
int t02;
int t03;

int count=0;
int dem=1;
int test;
int *ptr;

//Check Button
int run_BT;
int run_begin;
int test_BT;
int BT_ENTER=0, BT_UP=0, BT_DOWN=0, BT_ESC=0;

//gio phut giay
int hh, mm, ss;

CLCD_Name LCD1;
//uint8_t Count;
char LCD_send[16];

//hien thi lcd
void LCD_Running(void);

//hien thi lcd khi setup thoi gian
void LCD_Setup(int x);

void set(int *t, GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3);
void reset(int *t, GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3);
void check_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx); // Kiem tra nut nhan
void check_test(void);
void set_time(int *hh, int *mm, int *ss);
void CLCD_clearX(void); //xoa lcd th

void setup(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
	CLCD_4BIT_Init(&LCD1, 16,2, GPIOC, GPIO_PIN_8,GPIOC, GPIO_PIN_7,
															GPIOC, GPIO_PIN_6,GPIOB, GPIO_PIN_15,
															GPIOB, GPIO_PIN_14,GPIOB, GPIO_PIN_12);
	run_begin=0;
	dem=1;
	t=0;
	count=0;
	test=1;
	test_BT=0;
	run_BT=0;
	hh=0;mm=0;ss=0;
	while (test==1) 
	{
		CLCD_SetCursor(&LCD1,3,0);
		CLCD_WriteString(&LCD1,"Nhan ENTER");
	}
  while (1)
  {
		switch(test) 
		{
			case 0:
					setup();
					//t=0;
					//break;
			case 1:
					if(count==0)
					{
						set(&t, GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
					}
					if(count==1 && t>=2 )
					{
						set(&t, GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
					}
					if(count==2 && t>=t1 )
					{
						reset(&t, GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
					}
					if(count==3 && t>=t3 )
					{
						reset(&t, GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
					}
					if(count==4 && t>=60*t2)
					{
						count=0;
						t=0;
					}
					//break;
		}
		set_time(&hh, &mm, &ss);
		if(test==0) LCD_Setup(dem);
		else LCD_Running();
	}
}

void setup(void)
{
	if(BT_ENTER == 1)
	{
	}
	if( BT_ESC == 1)
	{
		if(dem==3) dem=1;
		else 			 dem++;
		if(dem==1 ) ptr=&t01;
		if(dem==2 ) ptr=&t02;
		if(dem==3 ) ptr=&t03;
		BT_ESC=0;
	}
	if( BT_DOWN == 1)
		{
			if(*ptr > 0)(*ptr)--;
			BT_DOWN=0;
		}
	if( BT_UP == 1)
		{
			(*ptr)++;
			BT_UP=0;
		}
	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_2)
	{
		if (test==0) test=1;
		else 				 test=0;
		if(test==1)
		{
			check_test();
			BT_ENTER=0;
			t=0;count=0;
			t1=t01;t2=t02;t3=t03;
		}
		else
		{
			t01=t1;
			t02=t2;
			t03=t3;
		//BT_ENTER = 1;
		//count=0;
		//t=0;
		}
		dem=1;
		ptr=&t01;
	}
	if(GPIO_Pin == GPIO_PIN_5 )
	{
		if(test==0) BT_ESC=1;
	}
	if(GPIO_Pin == GPIO_PIN_4 )
	{
		if(test==0 && HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)==0)
		{
			BT_DOWN=1;
			test_BT=1;
		}
		if(test==0 && HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)==1) 
		{
			test_BT=0;
			run_BT=0;
		}
	}
	if(GPIO_Pin == GPIO_PIN_3)
	{
		if(test==0 && HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==0) 
		{
			BT_UP=1;
			test_BT=1;
		}
		if(test==0 && HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1) 
		{
			test_BT=0;
			run_BT=0;
		}
	}
}

void LCD_Running(void)
{
	CLCD_SetCursor(&LCD1,0,0);
	CLCD_WriteString(&LCD1,"Running ");
	char h[3],m[3],s[3];
	
	if(hh<10) sprintf(h,"0%d:", hh);
	else sprintf(h,"%d:", hh);
	CLCD_SetCursor(&LCD1,8,0);
	CLCD_WriteString(&LCD1,h);
	
	if(mm<10) sprintf(m,"0%d:", mm);
	else sprintf(m,"%d:", mm);
	CLCD_SetCursor(&LCD1,11,0);
	CLCD_WriteString(&LCD1,m);
	
	if(ss<10) sprintf(s,"0%d", ss);
	else sprintf(s,"%d", ss);
	CLCD_SetCursor(&LCD1,14,0);
	CLCD_WriteString(&LCD1,s);
	
	sprintf(LCD_send,"T1:%d T2:%d T3:%d     ", t1,t2,t3);
	CLCD_SetCursor(&LCD1,0,1);
	CLCD_WriteString(&LCD1,LCD_send);
}

void LCD_Setup(int x)
{
	CLCD_SetCursor(&LCD1,0,0);
	if(x==1) 			CLCD_WriteString(&LCD1,"SET_T1 ");
	else if(x==2) CLCD_WriteString(&LCD1,"SET_T2 ");
	else			  	CLCD_WriteString(&LCD1,"SET_T3 ");
	
	char h[3],m[3],s[3];
	
	if(hh<10) sprintf(h,"0%d:", hh);
	else sprintf(h,"%d:", hh);
	CLCD_SetCursor(&LCD1,8,0);
	CLCD_WriteString(&LCD1,h);
	
	if(mm<10) sprintf(m,"0%d:", mm);
	else sprintf(m,"%d:", mm);
	CLCD_SetCursor(&LCD1,11,0);
	CLCD_WriteString(&LCD1,m);
	
	if(ss<10) sprintf(s,"0%d", ss);
	else sprintf(s,"%d", ss);
	CLCD_SetCursor(&LCD1,14,0);
	CLCD_WriteString(&LCD1,s);
	if(x==1) 			sprintf(LCD_send,"T1:%d Giay      ", t01);
	else if(x==2) sprintf(LCD_send,"T2:%d Phut      ", t02);
	if(x==3) 			sprintf(LCD_send,"T3:%d Giay      ", t03);	
	
	CLCD_SetCursor(&LCD1,0,1);
	CLCD_WriteString(&LCD1,LCD_send);
}

void set(int *t, GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3)
{
	HAL_GPIO_WritePin(GPIO1, GPIO_Pin1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO2, GPIO_Pin2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO3, GPIO_Pin3,GPIO_PIN_SET);
		count++;
		*t=0;
	//Onlcd(run, 0);
}
void reset(int *t, GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3)
{
	HAL_GPIO_WritePin(GPIO1, GPIO_Pin1,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO2, GPIO_Pin2,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO3, GPIO_Pin3,GPIO_PIN_RESET);
		count++;
		*t=0;
	//Onlcd(run, 0);
}

void set_time(int *hh, int *mm, int *ss)
{
	*hh=t/3600;
	*mm=(t-(*hh)*3600)/60;
	*ss=(t-(*hh)*3600-(*mm)*60);
	if(t>=86399)
	{
	t=0; *hh=0; *mm=0; *ss=0;
	}
}

void check_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx)
{
	run_BT=0;
	if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pinx)==0) test_BT=0;
	else test_BT=1;
}

void check_test(void)
{
	if(test == 0)
	{
		reset(&t, GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		reset(&t, GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		count=0;
		t=0;
		//CLCD_Clear(&LCD1);
		//CLCD_clearX();
		//lcd_on(dem);
	}
}

void CLCD_clearX(void)
{
	CLCD_SetCursor(&LCD1,0,0);
	CLCD_WriteString(&LCD1,"                 ");
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
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
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
	t++;
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

