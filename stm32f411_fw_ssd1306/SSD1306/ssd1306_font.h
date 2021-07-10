/*
 * ssd1306_font.h
 *
 *  Created on: 2021. 7. 11.
 *      Author: mokhwasomssi
 * 
 */


#ifndef __SSD1306_FONT_H__
#define __SSD1306_FONT_H__


#include <stdint.h>


typedef struct 
{
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;

} SSD1306_FONT;


extern SSD1306_FONT font6x8;



#endif /* __SSD1306_FONT_H__ */s