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
#include "u8g2.h"
#include "u8g_arm.h"
#include <stdio.h>
#include <stdlib.h>
#include "math.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* Квадрат */
typedef struct {
    float x;    // левый верхний угол
    float y;
    float vx;   // скорость по X (px/s)
    float vy;   // скорость по Y (px/s)
    uint8_t size;
    uint8_t num;
} Sprite;
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
   extern const uint8_t img[];
    
   u8g2_t u8g2;
   
   
 /* Параметры экрана */
#define SCREEN_W 160
#define SCREEN_H 160


   

   
   
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/* Заглушка: отрисовать квадрат на дисплее в позиции (ix, iy).
   Реализуйте в своём проекте: очистка предыдущего положения и рисование нового. */
void Display_DrawSquare(int ix, int iy, int size, int num)
{
 static int old_x[10], old_y[10], old_size[10];
 
 
  Set_window(old_x[num], old_y[num], old_size[num], old_size[num]);
  for(uint8_t j=0;j<100;j++)
        {           
          write_data(0x00);        
          write_data(0x00);          
          write_data(0x00); 
        }     
  
 
 
 
  Set_window(ix, iy, size, size);

  for(uint8_t j=0;j<100;j++)
        {           
          write_data(0xFF);        
          write_data(0xFF);          
          write_data(0xFF); 
        } 
  
  old_x[num] = ix;
  old_y[num] = iy;
  old_size[num] = size;

  
}  
  
  

/* Вспомогательная функция времени (HAL_GetTick из HAL) */
static inline uint32_t millis(void) { return HAL_GetTick(); }

/* Инициализация трёх спрайтов */
Sprite s[9];

void init_sprites(void) {
    s[0].num = 0; s[0].size = 18; s[0].x = 20.0f;  s[0].y = 30.0f;  s[0].vx = 32.0f;  s[0].vy = 3* 11.0f;   //6; 
    s[1].num = 1; s[1].size = 18; s[1].x = 60.0f;  s[1].y = 40.0f;  s[1].vx = -24.0f; s[1].vy = 3* 18.0f;   //12;
    s[2].num = 2; s[2].size = 18; s[2].x = 100.0f; s[2].y = 80.0f;  s[2].vx = 15.0f;  s[2].vy = 3* -20.0f;  //18;
    s[3].num = 3; s[3].size = 18; s[3].x = 10.0f;  s[3].y = 120.0f; s[3].vx = 40.0f;  s[3].vy = 3* -10.0f;  //8; 
    s[4].num = 4; s[4].size = 18; s[4].x = 130.0f; s[4].y = 20.0f;  s[4].vx = -18.0f; s[4].vy = 3* 22.0f;   //10;
    s[5].num = 5; s[5].size = 18; s[5].x = 50.0f;  s[5].y = 100.0f; s[5].vx = 12.0f;  s[5].vy = 3* -15.0f;  //14;
    s[6].num = 6; s[6].size = 18; s[6].x = 5.0f;   s[6].y = 5.0f;   s[6].vx = 28.0f;  s[6].vy = 3* 14.0f;   //7; 
    s[7].num = 7; s[7].size = 18; s[7].x = 140.0f; s[7].y = 130.0f; s[7].vx = -20.0f; s[7].vy = 3* -12.0f;  //9; 
    s[8].num = 8; s[8].size = 18; s[8].x = 80.0f;  s[8].y = 10.0f;  s[8].vx = 10.0f;  s[8].vy = 3* 25.0f;   //11;
}



/* Проверка пересечения двух AABB */
static inline int rects_intersect(Sprite *a, Sprite *b) {
    return !( (a->x + a->size <= b->x) || (b->x + b->size <= a->x) ||
              (a->y + a->size <= b->y) || (b->y + b->size <= a->y) );
}

/* Обработка столкновения — обмен скоростей (упрощение для равных масс) */
static void handle_collision(Sprite *a, Sprite *b) {
    /* Корректировка позиции чтобы убрать перекрытие: двигаем по нормали */
    float axc = a->x + a->size * 0.5f;
    float ayc = a->y + a->size * 0.5f;
    float bxc = b->x + b->size * 0.5f;
    float byc = b->y + b->size * 0.5f;
    float nx = axc - bxc;
    float ny = ayc - byc;
    /* если центры совпали, слегка сдвинем нормаль */
    if (nx == 0.0f && ny == 0.0f) { nx = 1.0f; ny = 0.0f; }
    /* нормализуем */
    float nlen = sqrtf(nx*nx + ny*ny);
    nx /= nlen; ny /= nlen;

    /* проектируем скорости на нормаль */
    float adot = a->vx * nx + a->vy * ny;
    float bdot = b->vx * nx + b->vy * ny;

    /* обмен компонент скоростей вдоль нормали (равные массы, упругое столкновение) */
    float adot_new = bdot;
    float bdot_new = adot;

    /* изменяем скорости, оставляя касательные компоненты неизменными */
    a->vx += (adot_new - adot) * nx;
    a->vy += (adot_new - adot) * ny;
    b->vx += (bdot_new - bdot) * nx;
    b->vy += (bdot_new - bdot) * ny;

    /* Убираем перекрытие: смещаем объекты вдоль нормали поровну */
    /* вычисляем минимальное смещение вдоль x и y для выхода из перекрытия */
    float overlap_x = (a->size + b->size) * 0.5f - fabsf(axc - bxc);
    float overlap_y = (a->size + b->size) * 0.5f - fabsf(ayc - byc);
    float push = fmaxf(overlap_x, overlap_y);
    if (push > 0.0f) {
        a->x += nx * (push * 0.5f);
        a->y += ny * (push * 0.5f);
        b->x -= nx * (push * 0.5f);
        b->y -= ny * (push * 0.5f);
    }
}









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

  
  
  
//      u8g2_Setup_uc1611_cg160160_f(&u8g2, U8G2_R2, u8x8_byte_hw_fsmc, u8x8_gpio_and_delay_template);     
//      u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
//      u8g2_SetPowerSave(&u8g2, 0); // wake up display
////      u8g2_ClearDisplay(&u8g2);       
////      u8g2_SetContrast(&u8g2, 40);   
//      u8g2_SetFontMode(&u8g2, 0);  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

#define DALAY_TIME      500 
  
  
  
    init();
    
    

    HAL_Delay(DALAY_TIME);

    
  write_com(0x81);			//electronic potentionmeter
  write_com(150);
    
  
   init_sprites();
   uint32_t last = millis();

  
   
   
   
    while(1)
    {      
       uint32_t now = millis();
        float dt = (now - last) / 1000.0f;
        if (dt <= 0.0f) continue;
        if (dt > 0.05f) dt = 0.05f;
        last = now;

      /* Обновление позиций и отражение от стен */
for (int i = 0; i < 9; ++i) {
    s[i].x += s[i].vx * dt;
    s[i].y += s[i].vy * dt;

    if (s[i].x < 0.0f) {
        s[i].x = -s[i].x;
        s[i].vx = -s[i].vx;
    }
    if (s[i].x + s[i].size > SCREEN_W) {
        float over = s[i].x + s[i].size - SCREEN_W;
        s[i].x -= 2.0f * over;
        s[i].vx = -s[i].vx;
    }
    if (s[i].y < 0.0f) {
        s[i].y = -s[i].y;
        s[i].vy = -s[i].vy;
    }
    if (s[i].y + s[i].size > SCREEN_H) {
        float over = s[i].y + s[i].size - SCREEN_H;
        s[i].y -= 2.0f * over;
        s[i].vy = -s[i].vy;
    }
}

/* Взаимные столкновения (все пары) */
for (int a = 0; a < 9; ++a) {
    for (int b = a + 1; b < 9; ++b) {
        if (rects_intersect(&s[a], &s[b])) {
            handle_collision(&s[a], &s[b]);
        }
    }
}

/* Рендер */
//Set_window(0, 0, 160, 160);
//display_white();
for (int i = 0; i < 9; ++i) {
    int ix = (int)(s[i].x + 0.5f);
    int iy = (int)(s[i].y + 0.5f);
    Display_DrawSquare(ix, iy, s[i].size, s[i].num);
}

        HAL_Delay(50);
    }

    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  
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
