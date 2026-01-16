/*

  u8x8_d_uc1698.c
  
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


  
  There are one controller:
  UC1698u		160x160
  
  
  
*/
#include "u8x8.h"

static const uint8_t u8x8_d_uc1698u_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ad), //on/off mode		/* display on, UC1698s */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t u8x8_d_uc1698u_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */  
  U8X8_C(0x0ac), //on/off mode		/* display off, UC1698s */     
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t u8x8_d_uc1698u_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0c4),            	        /* LCD Mapping */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t u8x8_d_uc1698u_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0c0),            	        /* LCD Mapping */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

//static const uint8_t lut_nibble[16][2] = {
//  {0x00, 0x00}, // 0b0000
//  {0x00, 0x0F}, // 0b0001
//  {0x00, 0xF0}, // 0b0010
//  {0x00, 0xFF}, // 0b0011
//  {0x0F, 0x00}, // 0b0100
//  {0x0F, 0x0F}, // 0b0101
//  {0x0F, 0xF0}, // 0b0110
//  {0x0F, 0xFF}, // 0b0111
//  {0xF0, 0x00}, // 0b1000
//  {0xF0, 0x0F}, // 0b1001
//  {0xF0, 0xF0}, // 0b1010
//  {0xF0, 0xFF}, // 0b1011
//  {0xFF, 0x00}, // 0b1100
//  {0xFF, 0x0F}, // 0b1101
//  {0xFF, 0xF0}, // 0b1110
//  {0xFF, 0xFF}, // 0b1111
//};



static const uint32_t lut_u32[256] = {
    0x00000000U,  // 0x00
    0x0000000FU,  // 0x01
    0x000000F0U,  // 0x02
    0x000000FFU,  // 0x03
    0x00000F00U,  // 0x04
    0x00000F0FU,  // 0x05
    0x00000FF0U,  // 0x06
    0x00000FFFU,  // 0x07
    0x0000F000U,  // 0x08
    0x0000F00FU,  // 0x09
    0x0000F0F0U,  // 0x0A
    0x0000F0FFU,  // 0x0B
    0x0000FF00U,  // 0x0C
    0x0000FF0FU,  // 0x0D
    0x0000FFF0U,  // 0x0E
    0x0000FFFFU,  // 0x0F
    0x000F0000U,  // 0x10
    0x000F000FU,  // 0x11
    0x000F00F0U,  // 0x12
    0x000F00FFU,  // 0x13
    0x000F0F00U,  // 0x14
    0x000F0F0FU,  // 0x15
    0x000F0FF0U,  // 0x16
    0x000F0FFFU,  // 0x17
    0x000FF000U,  // 0x18
    0x000FF00FU,  // 0x19
    0x000FF0F0U,  // 0x1A
    0x000FF0FFU,  // 0x1B
    0x000FFF00U,  // 0x1C
    0x000FFF0FU,  // 0x1D
    0x000FFFF0U,  // 0x1E
    0x000FFFFFU,  // 0x1F
    0x00F00000U,  // 0x20
    0x00F0000FU,  // 0x21
    0x00F000F0U,  // 0x22
    0x00F000FFU,  // 0x23
    0x00F00F00U,  // 0x24
    0x00F00F0FU,  // 0x25
    0x00F00FF0U,  // 0x26
    0x00F00FFFU,  // 0x27
    0x00F0F000U,  // 0x28
    0x00F0F00FU,  // 0x29
    0x00F0F0F0U,  // 0x2A
    0x00F0F0FFU,  // 0x2B
    0x00F0FF00U,  // 0x2C
    0x00F0FF0FU,  // 0x2D
    0x00F0FFF0U,  // 0x2E
    0x00F0FFFFU,  // 0x2F
    0x00FF0000U,  // 0x30
    0x00FF000FU,  // 0x31
    0x00FF00F0U,  // 0x32
    0x00FF00FFU,  // 0x33
    0x00FF0F00U,  // 0x34
    0x00FF0F0FU,  // 0x35
    0x00FF0FF0U,  // 0x36
    0x00FF0FFFU,  // 0x37
    0x00FFF000U,  // 0x38
    0x00FFF00FU,  // 0x39
    0x00FFF0F0U,  // 0x3A
    0x00FFF0FFU,  // 0x3B
    0x00FFFF00U,  // 0x3C
    0x00FFFF0FU,  // 0x3D
    0x00FFFFF0U,  // 0x3E
    0x00FFFFFFU,  // 0x3F
    0x0F000000U,  // 0x40
    0x0F00000FU,  // 0x41
    0x0F0000F0U,  // 0x42
    0x0F0000FFU,  // 0x43
    0x0F000F00U,  // 0x44
    0x0F000F0FU,  // 0x45
    0x0F000FF0U,  // 0x46
    0x0F000FFFU,  // 0x47
    0x0F00F000U,  // 0x48
    0x0F00F00FU,  // 0x49
    0x0F00F0F0U,  // 0x4A
    0x0F00F0FFU,  // 0x4B
    0x0F00FF00U,  // 0x4C
    0x0F00FF0FU,  // 0x4D
    0x0F00FFF0U,  // 0x4E
    0x0F00FFFFU,  // 0x4F
    0x0F0F0000U,  // 0x50
    0x0F0F000FU,  // 0x51
    0x0F0F00F0U,  // 0x52
    0x0F0F00FFU,  // 0x53
    0x0F0F0F00U,  // 0x54
    0x0F0F0F0FU,  // 0x55
    0x0F0F0FF0U,  // 0x56
    0x0F0F0FFFU,  // 0x57
    0x0F0FF000U,  // 0x58
    0x0F0FF00FU,  // 0x59
    0x0F0FF0F0U,  // 0x5A
    0x0F0FF0FFU,  // 0x5B
    0x0F0FFF00U,  // 0x5C
    0x0F0FFF0FU,  // 0x5D
    0x0F0FFFF0U,  // 0x5E
    0x0F0FFFFFU,  // 0x5F
    0x0FF00000U,  // 0x60
    0x0FF0000FU,  // 0x61
    0x0FF000F0U,  // 0x62
    0x0FF000FFU,  // 0x63
    0x0FF00F00U,  // 0x64
    0x0FF00F0FU,  // 0x65
    0x0FF00FF0U,  // 0x66
    0x0FF00FFFU,  // 0x67
    0x0FF0F000U,  // 0x68
    0x0FF0F00FU,  // 0x69
    0x0FF0F0F0U,  // 0x6A
    0x0FF0F0FFU,  // 0x6B
    0x0FF0FF00U,  // 0x6C
    0x0FF0FF0FU,  // 0x6D
    0x0FF0FFF0U,  // 0x6E
    0x0FF0FFFFU,  // 0x6F
    0x0FFF0000U,  // 0x70
    0x0FFF000FU,  // 0x71
    0x0FFF00F0U,  // 0x72
    0x0FFF00FFU,  // 0x73
    0x0FFF0F00U,  // 0x74
    0x0FFF0F0FU,  // 0x75
    0x0FFF0FF0U,  // 0x76
    0x0FFF0FFFU,  // 0x77
    0x0FFFF000U,  // 0x78
    0x0FFFF00FU,  // 0x79
    0x0FFFF0F0U,  // 0x7A
    0x0FFFF0FFU,  // 0x7B
    0x0FFFFF00U,  // 0x7C
    0x0FFFFF0FU,  // 0x7D
    0x0FFFFFF0U,  // 0x7E
    0x0FFFFFFFU,  // 0x7F
    0xF0000000U,  // 0x80
    0xF000000FU,  // 0x81
    0xF00000F0U,  // 0x82
    0xF00000FFU,  // 0x83
    0xF0000F00U,  // 0x84
    0xF0000F0FU,  // 0x85
    0xF0000FF0U,  // 0x86
    0xF0000FFFU,  // 0x87
    0xF000F000U,  // 0x88
    0xF000F00FU,  // 0x89
    0xF000F0F0U,  // 0x8A
    0xF000F0FFU,  // 0x8B
    0xF000FF00U,  // 0x8C
    0xF000FF0FU,  // 0x8D
    0xF000FFF0U,  // 0x8E
    0xF000FFFFU,  // 0x8F
    0xF00F0000U,  // 0x90
    0xF00F000FU,  // 0x91
    0xF00F00F0U,  // 0x92
    0xF00F00FFU,  // 0x93
    0xF00F0F00U,  // 0x94
    0xF00F0F0FU,  // 0x95
    0xF00F0FF0U,  // 0x96
    0xF00F0FFFU,  // 0x97
    0xF00FF000U,  // 0x98
    0xF00FF00FU,  // 0x99
    0xF00FF0F0U,  // 0x9A
    0xF00FF0FFU,  // 0x9B
    0xF00FFF00U,  // 0x9C
    0xF00FFF0FU,  // 0x9D
    0xF00FFFF0U,  // 0x9E
    0xF00FFFFFU,  // 0x9F
    0xF0F00000U,  // 0xA0
    0xF0F0000FU,  // 0xA1
    0xF0F000F0U,  // 0xA2
    0xF0F000FFU,  // 0xA3
    0xF0F00F00U,  // 0xA4
    0xF0F00F0FU,  // 0xA5
    0xF0F00FF0U,  // 0xA6
    0xF0F00FFFU,  // 0xA7
    0xF0F0F000U,  // 0xA8
    0xF0F0F00FU,  // 0xA9
    0xF0F0F0F0U,  // 0xAA
    0xF0F0F0FFU,  // 0xAB
    0xF0F0FF00U,  // 0xAC
    0xF0F0FF0FU,  // 0xAD
    0xF0F0FFF0U,  // 0xAE
    0xF0F0FFFFU,  // 0xAF
    0xF0FF0000U,  // 0xB0
    0xF0FF000FU,  // 0xB1
    0xF0FF00F0U,  // 0xB2
    0xF0FF00FFU,  // 0xB3
    0xF0FF0F00U,  // 0xB4
    0xF0FF0F0FU,  // 0xB5
    0xF0FF0FF0U,  // 0xB6
    0xF0FF0FFFU,  // 0xB7
    0xF0FFF000U,  // 0xB8
    0xF0FFF00FU,  // 0xB9
    0xF0FFF0F0U,  // 0xBA
    0xF0FFF0FFU,  // 0xBB
    0xF0FFFF00U,  // 0xBC
    0xF0FFFF0FU,  // 0xBD
    0xF0FFFFF0U,  // 0xBE
    0xF0FFFFFFU,  // 0xBF
    0xFF000000U,  // 0xC0
    0xFF00000FU,  // 0xC1
    0xFF0000F0U,  // 0xC2
    0xFF0000FFU,  // 0xC3
    0xFF000F00U,  // 0xC4
    0xFF000F0FU,  // 0xC5
    0xFF000FF0U,  // 0xC6
    0xFF000FFFU,  // 0xC7
    0xFF00F000U,  // 0xC8
    0xFF00F00FU,  // 0xC9
    0xFF00F0F0U,  // 0xCA
    0xFF00F0FFU,  // 0xCB
    0xFF00FF00U,  // 0xCC
    0xFF00FF0FU,  // 0xCD
    0xFF00FFF0U,  // 0xCE
    0xFF00FFFFU,  // 0xCF
    0xFF0F0000U,  // 0xD0
    0xFF0F000FU,  // 0xD1
    0xFF0F00F0U,  // 0xD2
    0xFF0F00FFU,  // 0xD3
    0xFF0F0F00U,  // 0xD4
    0xFF0F0F0FU,  // 0xD5
    0xFF0F0FF0U,  // 0xD6
    0xFF0F0FFFU,  // 0xD7
    0xFF0FF000U,  // 0xD8
    0xFF0FF00FU,  // 0xD9
    0xFF0FF0F0U,  // 0xDA
    0xFF0FF0FFU,  // 0xDB
    0xFF0FFF00U,  // 0xDC
    0xFF0FFF0FU,  // 0xDD
    0xFF0FFFF0U,  // 0xDE
    0xFF0FFFFFU,  // 0xDF
    0xFFF00000U,  // 0xE0
    0xFFF0000FU,  // 0xE1
    0xFFF000F0U,  // 0xE2
    0xFFF000FFU,  // 0xE3
    0xFFF00F00U,  // 0xE4
    0xFFF00F0FU,  // 0xE5
    0xFFF00FF0U,  // 0xE6
    0xFFF00FFFU,  // 0xE7
    0xFFF0F000U,  // 0xE8
    0xFFF0F00FU,  // 0xE9
    0xFFF0F0F0U,  // 0xEA
    0xFFF0F0FFU,  // 0xEB
    0xFFF0FF00U,  // 0xEC
    0xFFF0FF0FU,  // 0xED
    0xFFF0FFF0U,  // 0xEE
    0xFFF0FFFFU,  // 0xEF
    0xFFFF0000U,  // 0xF0
    0xFFFF000FU,  // 0xF1
    0xFFFF00F0U,  // 0xF2
    0xFFFF00FFU,  // 0xF3
    0xFFFF0F00U,  // 0xF4
    0xFFFF0F0FU,  // 0xF5
    0xFFFF0FF0U,  // 0xF6
    0xFFFF0FFFU,  // 0xF7
    0xFFFFF000U,  // 0xF8
    0xFFFFF00FU,  // 0xF9
    0xFFFFF0F0U,  // 0xFA
    0xFFFFF0FFU,  // 0xFB
    0xFFFFFF00U,  // 0xFC
    0xFFFFFF0FU,  // 0xFD
    0xFFFFFFF0U,  // 0xFE
    0xFFFFFFFFU,  // 0xFF
};








uint8_t u8x8_d_uc1698u_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  
  uint8_t data1=0, data2=0;
  
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x = x<<3;         // Fast multiplication by 8 with shift
      x /= 3;           //The X-axis is addressed using 1 RAM cell. 1 cell is 3 pixels in 4k RGB format.
      x += u8x8->x_offset;
      
      u8x8_cad_SendCmd(u8x8,0xF4); //start X
      u8x8_cad_SendCmd(u8x8,x);
      
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y = y<<3;
      u8x8_cad_SendCmd(u8x8,0xF5); //start y
      u8x8_cad_SendCmd(u8x8,y);
          
      u8x8_cad_SendCmd(u8x8,0xF6); //width
      u8x8_cad_SendCmd(u8x8,x+53); // 53=159/3 The X-axis is addressed using 1 RAM cell. 1 cell is 3 pixels in 4k RGB format.
      
      u8x8_cad_SendCmd(u8x8,0xF7); //height
      u8x8_cad_SendCmd(u8x8,y+u8x8->display_info->tile_height); //20 tile_height
      
      data1 = (x>>0)&0x0F;
      data2 = (x>>4)&0x0F;
      data2 |= 0x10;
      
      u8x8_cad_SendCmd(u8x8,data1);
      u8x8_cad_SendCmd(u8x8,data2);
      
      data1=0, data2=0;
      data1 = (y>>0)&0x0F;
      data1 |= 0x60;   
      
      data2 = (y>>4)&0x0F; 
      data2 |= 0x70;
      
      u8x8_cad_SendCmd(u8x8,data1);
      u8x8_cad_SendCmd(u8x8,data2);
            
      c = ((u8x8_tile_t *)arg_ptr)->cnt;
      c = c<<3;                                 // Fast multiplication by 8 with shift
      ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
      
      do
      {
//        for (uint16_t i=0; i<c; i++)
//        {  
//          const uint8_t *low  = lut_nibble[*ptr & 0x0F];
//          const uint8_t *high = lut_nibble[*ptr >> 4];
//          
//          uint8_t arr_4k[4];
//          arr_4k[3] = low[1];
//          arr_4k[2] = low[0];
//          arr_4k[1] = high[1];
//          arr_4k[0] = high[0];
//          
//          u8x8_cad_SendData(u8x8, 4, arr_4k);
//          
//          if ((i - 19) % 20 == 0) 
//          {
//            u8x8_cad_SendData(u8x8, 1, NULL);
//          }
//          ptr++;
//        }


        uint8_t buffer[80]; // 20 итераций × 4 байта
        uint16_t buf_pos = 0;
        
        for (uint16_t i = 0; i < c; i++)
        {  
          uint32_t val = lut_u32[*ptr++];
          buffer[buf_pos++] = (uint8_t)(val >> 24);
          buffer[buf_pos++] = (uint8_t)(val >> 16);
          buffer[buf_pos++] = (uint8_t)(val >> 8);
          buffer[buf_pos++] = (uint8_t)val;
          
          if (buf_pos == 80)
          {
            u8x8_cad_SendData(u8x8, 80, buffer);
            u8x8_cad_SendData(u8x8, 1, NULL);
            buf_pos = 0;
          }
        }
        // остаток
        if (buf_pos > 0)
        {
          u8x8_cad_SendData(u8x8, buf_pos, buffer);
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


static const u8x8_display_info_t u8x8_uc1698u_cg160160_display_info =
{
  /* chip_enable_level = */ 0,			/* use CS0 of the UC1611 */
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* pre_chip_disable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* reset_pulse_width_ms = */     20, 
  /* post_reset_wait_ms = */    200, 	/* uc1611 datasheet, page 67 */
  /* sda_setup_time_ns = */ 10,		/* uc1611 datasheet, page 64, actually 0 */
  /* sck_pulse_width_ns = */ 60,	/* half of cycle time  */
  /* sck_clock_hz = */ 8000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 30,	/* uc1611 datasheet, page 60 */
  /* write_pulse_width_ns = */ 80,	/* uc1611 datasheet, page 60 */
  /* tile_width = */ 20,		/* width of 20*8=160 pixel */
  /* tile_height = */ 20,
  /* default_x_offset = */ 37,
  /* flipmode_x_offset = */ 37,
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

static const uint8_t u8x8_d_uc1698u_cg160160_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0e2),		        /* system reset */
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
        
//        U8X8_C(0x0d6),			//64k color mode
        U8X8_C(0x0d5),			//4k color mode
 	      
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

	U8X8_C(0x089),			//RAM control /** Draw right then down **/

	
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
	U8X8_C(0x0a0),	                //160
  
        U8X8_C(0x0ac),                   //display OFF,select on/off mode.Green Enhance mode disable
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

/* cg160160 display */
uint8_t u8x8_d_uc1698u_cg160160(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* call common procedure first and handle messages there */
  if ( u8x8_d_uc1698u_common(u8x8, msg, arg_int, arg_ptr) == 0 )
  {
    /* msg not handled, then try here */
    switch(msg)
    {
      case U8X8_MSG_DISPLAY_SETUP_MEMORY:
	u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1698u_cg160160_display_info);
	break;
      case U8X8_MSG_DISPLAY_INIT:
	u8x8_d_helper_display_init(u8x8);
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_cg160160_init_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
	if ( arg_int == 0 )
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_powersave0_seq);
	else
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_powersave1_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
	if ( arg_int == 0 )
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_flip0_seq);
	  u8x8->x_offset = u8x8->display_info->default_x_offset;
	}
	else
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_flip1_seq);
	  u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
	}	
	break;
      default:
	return 0;		/* msg unknown */
    }
  }
  return 1;
}