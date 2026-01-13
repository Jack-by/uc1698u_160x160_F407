/*

  u8x8_d_uc1611.c
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  


  6 Nov 2016: Not yet finished
  
  There are two controller:
  UC1611s		160x256
  UC1611			160x240
  
  Differences:
  UC1611		0xa8 cmd: enables 80 display rows
  UC1611s	0xa8 cmd: controlls graylevels
  
  UC1611		0xc0 cmd: single byte command for LCD mapping control
  UC1611s	0xc0 cmd: double byte command for LCD mapping control
  
  
*/
#include "u8x8.h"

//#define GRAY_SCALE    
    
//#define COLOR_64K    




static const uint8_t u8x8_d_uc1611s_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
/**************************************************************************************************************************************************************/  
  
#ifdef GRAY_SCALE  
  U8X8_C(0x0af),   //grayscale mode		        /* display on, UC1698s */
#else
  U8X8_C(0x0ad), //on/off mode		                /* display on, UC1698s */
#endif  
  
/**************************************************************************************************************************************************************/  
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1611s_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
/**************************************************************************************************************************************************************/  

#ifdef GRAY_SCALE  
  U8X8_C(0x0ae),   //grayscale mode		        /* display on, UC1698s */
#else
//  U8X8_C(0x0ac), //on/off mode		        /* display on, UC1698s */
#endif   
  
/**************************************************************************************************************************************************************/  
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1611s_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CA(0x0c0, 0x004),            	/* LCD Mapping */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1611s_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CA(0x0c0, 0x002),            	/* LCD Mapping */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


static void Data_processing_uc1698(uint8_t in_bw, uint8_t* out4k)  //turns 1byte B/W data to 4k-color data(RRRR-GGGG-BBBB)   
{
uint8_t temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
uint8_t h11,h12,h13,h14,h15,h16,h17,h18,d1,d2,d3,d4;

		temp1=(in_bw&0x80)>>0;
		temp2=(in_bw&0x40)>>3;
		temp3=(in_bw&0x20)<<2;
		temp4=(in_bw&0x10)>>1;
		temp5=(in_bw&0x08)<<4;
		temp6=(in_bw&0x04)<<1;
		temp7=(in_bw&0x02)<<6;
		temp8=(in_bw&0x01)<<3;
		h11=temp1|temp1>>1|temp1>>2|temp1>>3;
		h12=temp2|temp2>>1|temp2>>2|temp2>>3;
		h13=temp3|temp3>>1|temp3>>2|temp3>>3;
		h14=temp4|temp4>>1|temp4>>2|temp4>>3;
		h15=temp5|temp5>>1|temp5>>2|temp5>>3;
		h16=temp6|temp6>>1|temp6>>2|temp6>>3;
		h17=temp7|temp7>>1|temp7>>2|temp7>>3;
		h18=temp8|temp8>>1|temp8>>2|temp8>>3;
		out4k[0]=h11|h12;
		out4k[1]=h13|h14;
		out4k[2]=h15|h16;
		out4k[3]=h17|h18;               
}





uint8_t u8x8_d_uc1611_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8x8->x_offset;
   
      u8x8_cad_SendCmd(u8x8, 0x000 | ((x&15)));
      u8x8_cad_SendCmd(u8x8, 0x010 | (x>>4) );
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      u8x8_cad_SendCmd(u8x8, 0x060 | (y&15));
      u8x8_cad_SendCmd(u8x8, 0x070 | (y>>4));
    
      c = ((u8x8_tile_t *)arg_ptr)->cnt;
      c *= 8;
      ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
      do
      {
        uint8_t arr_4k[4];
        
	for (uint16_t i=0; i<c; i++)
        {
          Data_processing_uc1698(*ptr, arr_4k);
          u8x8_cad_SendData(u8x8, 4, arr_4k);	/* note: SendData can not handle more than 255 bytes */ 
          ptr++;
        }
        
	arg_int--;
      } while( arg_int > 0 );
      
      u8x8_cad_EndTransfer(u8x8);
      break;
    /*	handled in the calling procedure 
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1611_128x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_uc1701_dogs102_init_seq);
      break;
    */
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int  );	/* uc1611 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    default:
      return 0;
  }
  return 1;
}

/*================================================*/
/* EA DOGM240 */


/*
  UC1611 has two chip select inputs (CS0 and CS1).
  CS0 is low active, CS1 is high active. It will depend on the display
  module whether the display has a is low or high active chip select.
*/

static const u8x8_display_info_t u8x8_uc1611_240x64_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* pre_chip_disable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 10, 	/* uc1611 datasheet, page 67 */
  /* sda_setup_time_ns = */ 10,		/* uc1611 datasheet, page 64, actually 0 */
  /* sck_pulse_width_ns = */ 60,	/* half of cycle time  */
  /* sck_clock_hz = */ 8000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 30,	/* uc1611 datasheet, page 60 */
  /* write_pulse_width_ns = */ 80,	/* uc1611 datasheet, page 60 */
  /* tile_width = */ 30,		/* width of 30*8=240 pixel */
  /* tile_height = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 240,
  /* pixel_height = */ 64
};

static const uint8_t u8x8_d_uc1611_ea_dogm240_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x02f),            			/* internal pump control */
  U8X8_CA(0x0f1, 63),			/* set COM end */
  U8X8_CA(0x0f2, 0x000),		/* display line start */
  U8X8_CA(0x0f3, 63),			/* display line end */
  U8X8_C(0x0a3),            			/* line rate */
  U8X8_CA(0x081, 0x0a4),		/* set contrast, EA default: 0x0b7 */
  
  //U8X8_C(0x0a9),            			/* display enable */

  U8X8_C(0x0d1),            			/* display pattern */  
  U8X8_C(0x089),            			/* auto increment */
  U8X8_CA(0x0c0, 0x004),            	/* LCD Mapping */
  U8X8_C(0x000),		                /* column low nibble */
  U8X8_C(0x010),		                /* column high nibble */  
  U8X8_C(0x060),		                /* page adr low */
  U8X8_C(0x070),		                /* page adr high */
  
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

/* UC1611s 240x64 display */
uint8_t u8x8_d_uc1611_ea_dogm240(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* call common procedure first and handle messages there */
  if ( u8x8_d_uc1611_common(u8x8, msg, arg_int, arg_ptr) == 0 )
  {
    /* msg not handled, then try here */
    switch(msg)
    {
      case U8X8_MSG_DISPLAY_SETUP_MEMORY:
	u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1611_240x64_display_info);
	break;
      case U8X8_MSG_DISPLAY_INIT:
	u8x8_d_helper_display_init(u8x8);
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_ea_dogm240_init_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
	if ( arg_int == 0 )
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_powersave0_seq);
	else
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_powersave1_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
	if ( arg_int == 0 )
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_flip0_seq);
	  u8x8->x_offset = u8x8->display_info->default_x_offset;
	}
	else
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_flip1_seq);
	  u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
	}	
	break;
      default:
	return 0;		/* msg unknown */
    }
  }
  return 1;
}

/*================================================*/
/* EA DOGXL240 */

static const uint8_t u8x8_d_uc1611_ea_dogxl240_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x02f),            			/* internal pump control */
  U8X8_CA(0x0f1, 0x07f),			/* set COM end */
  U8X8_CA(0x0f2, 0x000),		/* display line start */
  U8X8_CA(0x0f3, 127),		/* display line end */
  U8X8_C(0x0a3),            			/* line rate */
  U8X8_CA(0x081, 0x08f),		/* set contrast */
  
  //U8X8_C(0x0a9),            			/* display enable */

  U8X8_C(0x0d1),            			/* display pattern */  
  U8X8_C(0x089),            			/* auto increment */
  U8X8_CA(0x0c0, 0x004),            	/* LCD Mapping */
  U8X8_C(0x000),		                /* column low nibble */
  U8X8_C(0x010),		                /* column high nibble */  
  U8X8_C(0x060),		                /* page adr low */
  U8X8_C(0x070),		                /* page adr high */
  
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const u8x8_display_info_t u8x8_uc1611_240x128_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* pre_chip_disable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 10, 	/* uc1611 datasheet, page 67 */
  /* sda_setup_time_ns = */ 10,		/* uc1611 datasheet, page 64, actually 0 */
  /* sck_pulse_width_ns = */ 60,	/* half of cycle time  */
  /* sck_clock_hz = */ 8000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 30,	/* uc1611 datasheet, page 60 */
  /* write_pulse_width_ns = */ 80,	/* uc1611 datasheet, page 60 */
  /* tile_width = */ 30,		/* width of 30*8=240 pixel */
  /* tile_height = */ 16,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 240,
  /* pixel_height = */ 128
};

/* UC1611s 240x128 display */
uint8_t u8x8_d_uc1611_ea_dogxl240(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* call common procedure first and handle messages there */
  if ( u8x8_d_uc1611_common(u8x8, msg, arg_int, arg_ptr) == 0 )
  {
    /* msg not handled, then try here */
    switch(msg)
    {
      case U8X8_MSG_DISPLAY_SETUP_MEMORY:
	u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1611_240x128_display_info);
	break;
      case U8X8_MSG_DISPLAY_INIT:
	u8x8_d_helper_display_init(u8x8);
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_ea_dogxl240_init_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
	if ( arg_int == 0 )
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_powersave0_seq);
	else
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_powersave1_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
	if ( arg_int == 0 )
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_flip0_seq);
	  u8x8->x_offset = u8x8->display_info->default_x_offset;
	}
	else
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_flip1_seq);
	  u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
	}	
	break;
      default:
	return 0;		/* msg unknown */
    }
  }
  return 1;
}

/*================================================*/
/* EMERGING DISPLAY, EW50850FLWP 240x160 */
/* active high CS (CS1), UC1611 display  */

static const uint8_t u8x8_d_uc1611_ew50850_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x02f),            			/* internal pump control */
  U8X8_CA(0x0f1, 159),			/* set COM end */
  U8X8_CA(0x0f2, 0),			/* display line start */
  U8X8_CA(0x0f3, 159),			/* display line end */
  U8X8_C(0x0a3),            			/* line rate */
  U8X8_CA(0x081, 75),			/* set contrast */
  
  //U8X8_C(0x0a9),            			/* display enable */

  U8X8_C(0x0d2),            			/* gray level mode: 16 gray shades */  
  U8X8_C(0x089),            			/* auto increment */
  U8X8_C(0x0c0),            			/* LCD Mapping Bit 0: MSF, Bit 1: MX, Bit 2: MY */
  U8X8_C(0x000),		                /* column low nibble */
  U8X8_C(0x010),		                /* column high nibble */  
  U8X8_C(0x060),		                /* page adr low */
  U8X8_C(0x070),		                /* page adr high */
    
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const u8x8_display_info_t u8x8_uc1611_ew50850_display_info =
{
  /* chip_enable_level = */ 1,		/* active high */
  /* chip_disable_level = */ 0,
  
  /* post_chip_enable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* pre_chip_disable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 10, 	/* uc1611 datasheet, page 67 */
  /* sda_setup_time_ns = */ 10,		/* uc1611 datasheet, page 64, actually 0 */
  /* sck_pulse_width_ns = */ 60,	/* half of cycle time  */
  /* sck_clock_hz = */ 8000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 30,	/* uc1611 datasheet, page 60 */
  /* write_pulse_width_ns = */ 80,	/* uc1611 datasheet, page 60 */
  /* tile_width = */ 30,		/* width of 30*8=240 pixel */
  /* tile_height = */ 20,		/* height: 160 pixel */
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 240,
  /* pixel_height = */ 160
};

static const uint8_t u8x8_d_uc1611_alt_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0c0),				/* LCD Mapping Bit 0: MSF, Bit 1: MX, Bit 2: MY */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1611_alt_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0c6),				/* LCD Mapping Bit 0: MSF, Bit 1: MX, Bit 2: MY */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1611_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		                /* display on, UC1611 */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1611_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a8),		                /* display off, enter sleep mode, UC1611 */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


/* EW50850, 240x160 */
uint8_t u8x8_d_uc1611_ew50850(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c, i, v, m0, m1, ai;
  uint8_t *ptr;
  /* msg not handled, then try here */
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8x8->x_offset;
   
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y*=4;
      m0 = 1;
      m1 = 2;
      for( i = 0; i < 4; i++ )
      {
        u8x8_cad_SendCmd(u8x8, 0x000 | ((x&15)));
        u8x8_cad_SendCmd(u8x8, 0x010 | (x>>4) );
    
	u8x8_cad_SendCmd(u8x8, 0x060 | (y&15));
	u8x8_cad_SendCmd(u8x8, 0x070 | (y>>4));
      
	ai = arg_int;
	do
	{
	  c = ((u8x8_tile_t *)arg_ptr)->cnt;
	  c *= 8;
	  ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
	  while( c > 0 )
	  {
	    v = 0;
	    if ( *ptr & m0 )
	      v|= 0x0f;
	    if ( *ptr & m1 )
	      v|= 0xf0;
	    u8x8_cad_SendData(u8x8, 1, &v);	/* note: SendData can not handle more than 255 bytes */
	    c--;
            ptr++;
	  }
	  ai--;
	} while( ai > 0 );
	
	m0 <<= 2;
	m1 <<= 2;
	y++;
      }
      u8x8_cad_EndTransfer(u8x8);
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1611_ew50850_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_ew50850_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_alt_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_alt_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }	
      break;
#ifdef U8X8_WITH_SET_CONTRAST
  case U8X8_MSG_DISPLAY_SET_CONTRAST:
    u8x8_cad_StartTransfer(u8x8);
    u8x8_cad_SendCmd(u8x8, 0x081 );
    u8x8_cad_SendArg(u8x8, arg_int  );	/* uc1611 has range from 0 to 255 */
    u8x8_cad_EndTransfer(u8x8);
    break;
#endif
    default:
      return 0;		/* msg unknown */
  }
  return 1;
}


/*================================================*/
/* CG160160D, http://www.cloverdisplay.com/pdf/CG160160D.pdf  */

/*
  UC1611 has two chip select inputs (CS0 and CS1).
  CS0 is low active, CS1 is high active. It will depend on the display
  module whether the display has a is low or high active chip select.

  Connect CS1 to 3.3V and CS0 to GPIO
*/


/** ETO DANNIE DLYA COMUNICACII S DISPLEEM              **/



static const u8x8_display_info_t u8x8_uc1611_cg160160_display_info =
{
  /* chip_enable_level = */ 0,			/* use CS0 of the UC1611 */
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* pre_chip_disable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* reset_pulse_width_ms = */ 20, 
  /* post_reset_wait_ms = */ 200, 	/* uc1611 datasheet, page 67 */
  /* sda_setup_time_ns = */ 10,		/* uc1611 datasheet, page 64, actually 0 */
  /* sck_pulse_width_ns = */ 60,	/* half of cycle time  */
  /* sck_clock_hz = */ 8000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 30,	/* uc1611 datasheet, page 60 */
  /* write_pulse_width_ns = */ 80,	/* uc1611 datasheet, page 60 */
  /* tile_width = */ 20,		/* width of 20*8=160 pixel */
  /* tile_height = */ 20,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 160,
  /* pixel_height = */ 160
};

/*
System Reset: E2H 	--> DONE
Set Temp. Compensation: 24H --> DONE
Set up LCD format specific parameters MX,MY,etc(double-byte command): C0H,04H  --> FLIP0
Set line rate: A3H --> DONE
Set Pump Control (internal Vlcd): 2FH --> DONE
Set Isolation Clock Front (3 bytes command): 82H, 13H, 01H  --> DONE
Set Isolation Clock Back (3 bytes command): 82H, 14H, 00H  --> DONE
Set LCD Bias Ratio: EAH 
LCD Specific Operation Voltage Setting (double-byte command): 81H, 90H --> DONE
Set RAM Address Control: 80H --> DOES NOT MAKE SENSE
Set Page Addr. MSB: 72H 		--> DONE
Set Page Addr. LSB : 60H 		--> DONE
Set Column Addr. LSB: 00H 		--> DONE
Set Column Addr.MSB: 10H 		--> DONE
Window Program Enable : F8H 		--> NOT REQURED
Window Starting Column (double-byte command): F4H , 00H --> NOT REQURED
Window Ending Column (double-byte command): F6H, 9FH --> NOT REQURED
Set one bit for one pixel: D1H 		--> DONE
Set Display Enable: A9H 
*/

static const uint8_t u8x8_d_uc1611_cg160160_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0e2),				/* system reset */
  U8X8_DLY(10),
  
  /*power control*/					
	U8X8_C(0x0e9),			//Bias Ratio:1/10 bias
	U8X8_C(0x02b),			//power control set as internal power
	U8X8_C(0x024),			//set temperate compensation as 0%
	U8X8_C(0x081),			//electronic potentionmeter
	U8X8_C(0x096),	                //start contrast level

	/*display control*/
	U8X8_C(0x0a4),			//all pixel off 
	U8X8_C(0x0a6),			//inverse display off

	/*lcd control*/
	U8X8_C(0x0c4),			//Set LCD Maping Control (MY=1, MX=0)
	U8X8_C(0x0a1),			//line rate 15.2klps
	U8X8_C(0x0d1),			//rgb-rgb
        

#ifdef COLOR_64K  
        U8X8_C(0x0d6),			//64k color mode
#else
        U8X8_C(0x0d5),			//4k color mode
#endif  	
      
	U8X8_C(0x084),			//12:partial display control disable


	/*n-line inversion*/
	U8X8_C(0x0c8),
	U8X8_C(0x010),			//enable NIV

	/*com scan fuction*/
	U8X8_C(0x0da),			//enable FRC,PWM,LRM sequence

	/*window*/
	U8X8_C(0x0f4),			//wpc0:column
	U8X8_C(0x025),			//start from 112
	U8X8_C(0x0f6),			//wpc1
	U8X8_C(0x05A),			//end:272

	U8X8_C(0x0f5),			//wpp0:row
	U8X8_C(0x000),			//start from 0
	U8X8_C(0x0f7),			//wpp1
	U8X8_C(0x09F),			//end 160

	U8X8_C(0x0f8),			//inside mode 

	U8X8_C(0x089),			//RAM control

	
	/*scroll line*/
	U8X8_C(0x040),			//low bit of scroll line
	U8X8_C(0x050),			//high bit of scroll line

	U8X8_C(0x090),			//14:FLT,FLB set
	U8X8_C(0x000),

	/*partial display*/
	U8X8_C(0x084),			//12,set partial display control:off
	U8X8_C(0x0f1),			//com end
	U8X8_C(0x09f),			//160
	U8X8_C(0x0f2),			//display start 
	U8X8_C(0x000),				//0
	U8X8_C(0x0f3),			//display end
	U8X8_C(160),	//????????????????????160
  
        U8X8_C(0x0ac),                   //display OFF,select on/off mode.Green Enhance mode disable
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

/* cg160160 display */
uint8_t u8x8_d_uc1611_cg160160(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* call common procedure first and handle messages there */
  if ( u8x8_d_uc1611_common(u8x8, msg, arg_int, arg_ptr) == 0 )
  {
    /* msg not handled, then try here */
    switch(msg)
    {
      case U8X8_MSG_DISPLAY_SETUP_MEMORY:
	u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1611_cg160160_display_info);
	break;
      case U8X8_MSG_DISPLAY_INIT:
	u8x8_d_helper_display_init(u8x8);
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_cg160160_init_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
	if ( arg_int == 0 )
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_powersave0_seq);
	else
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_powersave1_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
	if ( arg_int == 0 )
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_flip0_seq);
	  u8x8->x_offset = u8x8->display_info->default_x_offset;
	}
	else
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_flip1_seq);
	  u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
	}	
	break;
      default:
	return 0;		/* msg unknown */
    }
  }
  return 1;
}


/*================================================*/
/* CI064-4073-06 (Intelligent Display Solutions), IDS4073*/
/* https://docs.rs-online.com/7e6e/0900766b8156b018.pdf */

static const uint8_t u8x8_d_uc1611_ids4073_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x02f),            			/* internal pump control */
  U8X8_CA(0x0f1, 0x07f),			/* set COM end */
  U8X8_CA(0x0f2, 0x000),		/* display line start */
  U8X8_CA(0x0f3, 127),		/* display line end */
  U8X8_C(0x0a3),            			/* line rate */
  U8X8_CA(0x081, 0x08f),		/* set contrast */
  
  //U8X8_C(0x0a9),            			/* display enable */

  U8X8_C(0x0d1),            			/* display pattern */  
  U8X8_C(0x089),            			/* auto increment */
  U8X8_CA(0x0c0, 0x004),            	/* LCD Mapping */
  U8X8_C(0x000),		                /* column low nibble */
  U8X8_C(0x010),		                /* column high nibble */  
  U8X8_C(0x060),		                /* page adr low */
  U8X8_C(0x070),		                /* page adr high */
  
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


static const u8x8_display_info_t u8x8_uc1611_256x128_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* pre_chip_disable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 10, 	/* uc1611 datasheet, page 67 */
  /* sda_setup_time_ns = */ 10,		/* uc1611 datasheet, page 64, actually 0 */
  /* sck_pulse_width_ns = */ 60,	/* half of cycle time  */
  /* sck_clock_hz = */ 8000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 30,	/* uc1611 datasheet, page 60 */
  /* write_pulse_width_ns = */ 80,	/* uc1611 datasheet, page 60 */
  /* tile_width = */ 32,		/* width of 32*8=256 pixel */
  /* tile_height = */ 16,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 256,
  /* pixel_height = */ 128
};

/* UC1611s 256x128 display */
uint8_t u8x8_d_uc1611_ids4073(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8x8->x_offset;
   
      u8x8_cad_SendCmd(u8x8, 0x000 | ((x&15)));
      u8x8_cad_SendCmd(u8x8, 0x010 | (x>>4) );
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      u8x8_cad_SendCmd(u8x8, 0x060 | (y&15));
      u8x8_cad_SendCmd(u8x8, 0x070 | (y>>4));
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        /* SendData can not handle more than 255 bytes */
        if ( c > 31 )
        {
          u8x8_cad_SendData(u8x8, 248, ptr); 	/* 31*8=248 */
          ptr+=248;
          c -= 31;
        }
        
        u8x8_cad_SendData(u8x8, c*8, ptr); 	
        arg_int--;
      } while( arg_int > 0 );
      
      u8x8_cad_EndTransfer(u8x8);
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int  );	/* uc1611 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1611_256x128_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611_ids4073_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1611s_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }	
      break;
    default:
      return 0;		/* msg unknown */
  }
  return 1;
}
