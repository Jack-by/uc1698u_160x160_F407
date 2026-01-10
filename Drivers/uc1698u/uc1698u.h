



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UC1698U
#define __UC1698U


#include "main.h"
#include "stdint.h"
#include "stm32f4xx_hal.h"


#define ADDR_CMD  *(uint8_t*)0x60000000
#define ADDR_DATA *(uint8_t*)0x60010000







void UC1698_writecommand8(uint8_t com);//command write
void UC1698_writedata8(uint8_t data);//data write

void init(void);
void test(void);
void display_black(void);
void display_white(void);
void frame(void);
void all_v1(void);
void all_v2(void);
void text_dot(uint8_t data1, uint8_t data2);
void delayms(uint32_t t);
void write_com(uint32_t com);
void write_data(uint32_t data);
void Data_processing(uint8_t temp);
void Data_processing_enh(const uint8_t *pic);
void adj_Contrast(uint8_t Contrast_level);
void display_Contrast_level(uint8_t number);
void Write_number(uint8_t x, uint8_t y, const uint8_t *n, uint8_t k);
void display_address(void);
void display_gray(void);

void disppic(const uint8_t *pic);
void disppic_enh(const uint8_t *pic);
void Set_window(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void display_start_window(uint8_t x, uint8_t y);





















#endif
