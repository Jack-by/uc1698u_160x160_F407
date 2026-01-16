
#include "u8g_arm.h"

uint8_t control = 0;

/**
#define ADDR_CMD  *(uint8_t*)0x60000000
#define ADDR_DATA *(uint8_t*)0x60010000
**/

uint8_t *FSMC_PTR=NULL;


void Fsmc_out (uint8_t data); 



/* set TIM5 to run at DELAY_TIM_FREQUENCY */
/*
void delay_init( void )
{
  // Enable timer clock  - use TIMER5 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


  // Time base configuration 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = (SYS_CLK / DELAY_TIM_FREQUENCY) - 1;
  TIM_TimeBaseStructure.TIM_Period = UINT16_MAX;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

 // Enable counter 
  TIM_Cmd(TIM2, ENABLE);
}
*/

/*
  Delay by the provided number of micro seconds.
  Limitation: "us" * System-Freq in MHz must now overflow in 32 bit.
  Values between 0 and 1.000.000 (1 second) are ok.
*/
void delay_micro_seconds(uint32_t us)
{
	/*
	TIM_SetCounter(TIM2, 0);
	  // use 16 bit count wrap around 
	  while((uint16_t)(TIM2->CNT) <= us);
	*/
}

/*========================================================================*/
/*
  The following delay procedures must be implemented for u8glib

  void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
  void u8g_MicroDelay(void)		Delay be one microsecond
  void u8g_10MicroDelay(void)	Delay by 10 microseconds

*/

//void u8g_Delay(uint16_t val)
//{
//    
//  delay_micro_seconds(1000UL*(uint32_t)val);
//}

void u8g_MicroDelay(void)
{
//  LL_TIM_ClearFlag_UPDATE(TIM8);
//  LL_TIM_SetCounter(TIM8, 0x00);
//  LL_TIM_EnableCounter(TIM8);
//   
//  while ( LL_TIM_IsActiveFlag_UPDATE(TIM8) == RESET);
//  LL_TIM_ClearFlag_UPDATE(TIM8);
  
//  for (uint16_t i=0; i<10; i++);
  
  
}

void u8g_10MicroDelay(void)
{
//  for (uint8_t i=0; i<10; i++)
//  {
//    u8g_MicroDelay();
//  }
}

//**************************  SW_SPI  *****************************************************
//*************************************************************************************
//uint8_t u8x8_gpio_and_delay_template(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
//{
//  switch(msg)
//  {
//    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
//      break;							// can be used to setup pins
//    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
//     
////       u8g_MicroDelay();
//      break;    
//    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
//       u8g_MicroDelay();
//      break;
//      
//    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
//       u8g_10MicroDelay();
//      break;
//      
//    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
//      for(volatile uint8_t i=0; i<100; i++)
//        u8g_10MicroDelay();
//      break;
//      
//    case U8X8_MSG_DELAY_I2C:				// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
//      break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
//    case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
//      if ( arg_int == 0 )               
//         LL_GPIO_ResetOutputPin(SCK_GPIO_Port, SCK_Pin);
//        else
//         LL_GPIO_SetOutputPin(SCK_GPIO_Port, SCK_Pin);      
//      break;
//      
//    case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
//      if ( arg_int == 0 )               
//         LL_GPIO_ResetOutputPin(MOSI_GPIO_Port, MOSI_Pin);
//        else
//         LL_GPIO_SetOutputPin(MOSI_GPIO_Port, MOSI_Pin);      
//      break;
//      
//    case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
//       if ( arg_int == 0 )               
//         LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin);
//        else
//          LL_GPIO_SetOutputPin(CS_GPIO_Port, CS_Pin);
//	    
//      break;
//   
//  case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
//       if (arg_int == 0)     
//         LL_GPIO_ResetOutputPin(A0_GPIO_Port, A0_Pin);
//        else        
//         LL_GPIO_SetOutputPin(A0_GPIO_Port, A0_Pin);              
//      break;
//      
//    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
//       if (arg_int == 0)      
//         LL_GPIO_ResetOutputPin(RESET_GPIO_Port, RESET_Pin);
//        else        
//         LL_GPIO_SetOutputPin(RESET_GPIO_Port, RESET_Pin);             
//      break;
//      
//    case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
//      break;
//    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
//      break;							// arg_int=1: Input dir with pullup high for I2C clock pin
//    case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
//      break;							// arg_int=1: Input dir with pullup high for I2C data pin
//    case U8X8_MSG_GPIO_MENU_SELECT:
//      u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
//      break;
//    case U8X8_MSG_GPIO_MENU_NEXT:
//      u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
//      break;
//    case U8X8_MSG_GPIO_MENU_PREV:
//      u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
//      break;
//    case U8X8_MSG_GPIO_MENU_HOME:
//      u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
//      break;
//    default:
//      u8x8_SetGPIOResult(u8x8, 1);			// default return value
//      break;
//  }
//  return 1;
//}




//**************************  HW_FSMC  *****************************************************
//*************************************************************************************
uint8_t u8x8_gpio_and_delay_template(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
      break;							// can be used to setup pins
    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second     
//      __NOP(); __NOP();
      break;    
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
//      __NOP(); __NOP();
      break;
      
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
//      __NOP(); __NOP();
      break;
      
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
     HAL_Delay(arg_int);
      break;
      
    case U8X8_MSG_DELAY_I2C:				// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
      break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
    //case U8X8_MSG_GPIO_SPI_CLOCK:
      break;
    case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
    //case U8X8_MSG_GPIO_SPI_DATA:
      break;
    case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
      break;
   
  case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
       if (arg_int == 0)   
         FSMC_PTR=ADDR_CMD_U8G2;        
        else        
          FSMC_PTR=ADDR_DATA_U8G2;              
      break;
      
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
       if (arg_int == 0)      
         LL_GPIO_ResetOutputPin(RESET_GPIO_Port, RESET_Pin);
        else        
         LL_GPIO_SetOutputPin(RESET_GPIO_Port, RESET_Pin);             
      break;
      
    case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
      break;							// arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
      break;							// arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
//      u8x8_SetGPIOResult(u8x8, LL_GPIO_IsInputPinSet(BUTT_SELECT_GPIO_Port, BUTT_SELECT_Pin));      
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
//      u8x8_SetGPIOResult(u8x8, LL_GPIO_IsInputPinSet(BUTT_NEXT_GPIO_Port, BUTT_NEXT_Pin));
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
//      u8x8_SetGPIOResult(u8x8, LL_GPIO_IsInputPinSet(BUTT_PREV_GPIO_Port, BUTT_PREV_Pin));
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
//      u8x8_SetGPIOResult(u8x8, LL_GPIO_IsInputPinSet(BUTT_HOME_GPIO_Port, BUTT_HOME_Pin));
      break;
    case U8X8_MSG_GPIO_MENU_UP:
//      u8x8_SetGPIOResult(u8x8, LL_GPIO_IsInputPinSet(BUTT_UP_GPIO_Port, BUTT_UP_Pin));
      break;
    case U8X8_MSG_GPIO_MENU_DOWN:
//      u8x8_SetGPIOResult(u8x8, LL_GPIO_IsInputPinSet(BUTT_DOWN_GPIO_Port, BUTT_DOWN_Pin));
      break;
      
      
      
    default:
      u8x8_SetGPIOResult(u8x8, 1);			// default return value
      break;
  }
  return 1;
}



//*********************************************************************************
//*********************************************************************************
uint8_t u8x8_byte_hw_fsmc(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) 
{
  uint8_t *data;
//  uint8_t internal_spi_mode; 
  switch(msg) {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 ) 
      {        
        Fsmc_out((uint8_t)*data);
        data++;
        arg_int--;
      }            
      break;
    case U8X8_MSG_BYTE_INIT:
      
      /*
      U8X8_MSG_CAD_INIT
      no args
      call U8X8_MSG_BYTE_INIT
      setup default values for the I/O lines
      */
      
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:      
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:            
      break;
    default:
      return 0;
  }  
  return 1;
}



//*********************************************************************************
//*********************************************************************************
void Fsmc_out (uint8_t data) 
{
    *FSMC_PTR = data;
}














