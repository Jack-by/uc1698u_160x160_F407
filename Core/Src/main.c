/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "uc1698u.h"  
    
    
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

   extern uint8_t const num[];
   extern uint8_t const pic_data[];
   extern uint8_t const pic_data1[];
   extern uint8_t const pic_data2[];
   extern uint8_t const pic_data3[];
   extern const uint8_t img159x160[];
    
    
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */









/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */


#define DALAY_TIME      500 
  
  
  
    init();
    
    
     display_white();
    HAL_Delay(DALAY_TIME);
//    adj_Contrast (200 );
//    HAL_Delay(DALAY_TIME);
    
    
//    display_gray( );
//    HAL_Delay(3000);
    
//    Data_processing(0xFF);
//    Data_processing(0x00);
//    Data_processing(0xFF);
    
  write_com(0x81);			//electronic potentionmeter
  write_com(220);
    
    Set_window(5, 5, 127, 127);
    display_start_window(6, 6);
//    display_address();
    disppic_enh(img159x160);
    
     write_com(0xa7);
    
//    write_data(d2);
//    write_data(d3);
//    write_data(d4);

    
   
//     write_data(0xFF);
    while(1);
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {    
    display_address();
//    display_black();
//    HAL_Delay(DALAY_TIME);
//    display_address();
//    display_white();
//    HAL_Delay(DALAY_TIME);
//    display_address();
//    text_dot(0xf0,0x0f);
//    HAL_Delay(DALAY_TIME);
//    text_dot(0x0f,0xf0);
//    HAL_Delay(DALAY_TIME);
//    text_dot(0xff,0x00);
//    HAL_Delay(DALAY_TIME);
//    text_dot(0x00,0xff);
//    HAL_Delay(DALAY_TIME);
//    text_dot(0xf0,0xf0);
//    HAL_Delay(DALAY_TIME);
//    text_dot(0x0f,0x0f);
//    HAL_Delay(DALAY_TIME);
//    display_address();
    
   

    disppic(pic_data);
    HAL_Delay(DALAY_TIME);
    
//    write_com(0xa7);
//    HAL_Delay(DALAY_TIME);    
//    write_com(0xa6);
    
    display_address();
    disppic(pic_data1);
    HAL_Delay(DALAY_TIME);
    
//    write_com(0xa7);
//    HAL_Delay(DALAY_TIME);    
//    write_com(0xa6);
    
    display_address();
    disppic(pic_data2);
    HAL_Delay(DALAY_TIME);
    
//    write_com(0xa7);
//    HAL_Delay(DALAY_TIME);
//    write_com(0xa6);
    
    display_address();
    disppic(pic_data3);
    HAL_Delay(DALAY_TIME);
    
//    write_com(0xa7);
//    HAL_Delay(DALAY_TIME);
//    write_com(0xa6);
    
    display_address();
//    display_white();
    HAL_Delay(DALAY_TIME);
//    adj_Contrast (160 );
//    HAL_Delay(DALAY_TIME);
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(168000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
