/*
 * ssd1306.c
 *
 *  Created on: 2021. 6. 30.
 *      Author: mokhwasomssi
 * 
 */


#include "ssd1306.h"
#include <string.h> // memcpy

/* SSD1306 Variable */
uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];
SSD1306_CURSOR cursor;


/* I2C Write Function */
void ssd1306_write_command(uint8_t command)
{
    HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_SA_WRITE, SSD1306_CONTROL_BYTE_COMMAND, 1, &command, 1, 1000);
}

void ssd1306_write_data(uint8_t* buffer, uint16_t size)
{
    HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_SA_WRITE, SSD1306_CONTROL_BYTE_DATA, 1, buffer, size, 1000);
}


/* Charge Bump Setting */
void charge_bump_setting(uint8_t charge_bump)
{
    ssd1306_write_command(CHARGE_BUMP_SETTING);
    ssd1306_write_command(charge_bump);
}

/* Fundamental */
void set_contrast_control(uint8_t value)
{
    ssd1306_write_command(SET_CONTRAST_CONTROL);
    ssd1306_write_command(value);
}

void entire_display_off()
{
    ssd1306_write_command(ENTIRE_DISPLAY_OFF);
}

void entire_display_on()
{
    ssd1306_write_command(ENTIRE_DISPLAY_ON);
}

void set_normal_display()
{
    ssd1306_write_command(SET_NORMAL_DISPLAY);
}

void set_inverse_display()
{
    ssd1306_write_command(SET_INVERSE_DISPLAY);
}

void set_display_on()
{
    ssd1306_write_command(SET_DISPLAY_ON);
}

void set_display_off()
{
    ssd1306_write_command(SET_DISPLAY_OFF);
}


/* Scrolling Command */
// 안써용~


/* Addressing Setting */
void set_lower_column_start_address_for_page_addressing_mode(uint8_t addr)
{
    ssd1306_write_command(addr);
}

void set_higher_column_start_address_for_page_addressing_mode(uint8_t addr)
{
    ssd1306_write_command(addr);
}

void set_memory_addressing_mode(uint8_t mode)
{
    ssd1306_write_command(SET_MEMORY_ADDRESSING_MODE);
    ssd1306_write_command(mode);
}

void set_column_address(uint8_t start, uint8_t end)
{
    ssd1306_write_command(SET_COLUMN_ADDRESS);
    ssd1306_write_command(start);
    ssd1306_write_command(end);
}

void set_page_address(uint8_t start, uint8_t end)
{
    ssd1306_write_command(SET_PAGE_ADDRESS);
    ssd1306_write_command(start);
    ssd1306_write_command(end);
}

void set_page_start_address_for_page_addressing_mode(uint8_t page)  // 0xB0(page0) ~ 0xB7(page7)
{
    ssd1306_write_command(page);
}


/* Hardware Configuration */
void set_display_start_line(uint8_t start_line)
{
    ssd1306_write_command(start_line);
}

void set_segment_remap(uint8_t mapping)
{
    ssd1306_write_command(mapping);
}

void set_multiplex_ratio(uint8_t mux)
{
    ssd1306_write_command(SET_MULTIPLEX_RATIO);
    ssd1306_write_command(mux);
}

void set_com_output_scan_direction(uint8_t mode)
{
    ssd1306_write_command(mode);
}

void set_display_offset(uint8_t vertical_shift)
{
    ssd1306_write_command(SET_DISPLAY_OFFSET);
    ssd1306_write_command(vertical_shift);
}

void set_com_pins_hardware_config(uint8_t com_pin_config, uint8_t com_left_right_remap)
{
    uint8_t buffer = 0x02 | (com_pin_config << 4) | (com_left_right_remap << 5);

    ssd1306_write_command(SET_COM_PINS_HARDWARE_CONFIG);
    ssd1306_write_command(buffer);
}


/* Timing & Driving Scheme Setting */
void set_display_clock_divide_ratio_and_osc_freq(uint8_t divide_ratio, uint8_t osc_freq)
{
    uint8_t buffer = (osc_freq << 4) | divide_ratio;

    ssd1306_write_command(SET_DISPLAY_CLOCK_DIVIDE_RATIO_AND_OSC_FREQ);
    ssd1306_write_command(buffer);
}

void set_pre_charge_period(uint8_t phase_1, uint8_t phase_2)
{
    uint8_t buffer = (phase_2 << 4) | phase_1;

    ssd1306_write_command(SET_PRE_CHARGE_PERIOD);
    ssd1306_write_command(buffer);
}

void set_v_comh_deselect_level(uint8_t deselect_level)
{
    ssd1306_write_command(SET_V_COMH_DESELECT_LEVEL);
    ssd1306_write_command(deselect_level);
}


/* SSD1306 Function */
void ssd1306_init()
{
    // SSD1306 App Note 5p
    set_multiplex_ratio(63);

    set_display_offset(0);

    set_display_start_line(0x40);

    set_segment_remap(0xA1);

    set_com_output_scan_direction(0xC8);

    set_com_pins_hardware_config(1, 0);

    set_contrast_control(0x7F);

    entire_display_off();

    set_normal_display();

    set_display_clock_divide_ratio_and_osc_freq(0, 8);

    charge_bump_setting(0x14);

    set_display_on();
}

void ssd1306_update_screen()
{
    for(int i = 0; i < SSD1306_PAGE; i ++)
    {
        set_page_start_address_for_page_addressing_mode(0xB0 + i);
        set_lower_column_start_address_for_page_addressing_mode(0x00);
        set_higher_column_start_address_for_page_addressing_mode(0x10);

        ssd1306_write_data(&ssd1306_buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
    }
}

void ssd1306_black_screen()
{
    for(int i = 0; i < SSD1306_BUFFER_SIZE; i++)
    {
        ssd1306_buffer[i] = 0x00;
    }

    ssd1306_update_screen();
}

void ssd1306_white_screen()
{
    for(int i = 0; i < SSD1306_BUFFER_SIZE; i++)
    {
        ssd1306_buffer[i] = 0xFF;
    }

    ssd1306_update_screen();
}

void ssd1306_black_pixel(uint8_t x, uint8_t y)
{
    // (y / 8) * SSD1306_WIDTH : page
    // y % 8 : data bit D0 - D7
    ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}

void ssd1306_white_pixel(uint8_t x, uint8_t y)
{
    // (y / 8) * SSD1306_WIDTH : page
    // y % 8 : data bit D0 - D7
    ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
}

char ssd1306_write_char(SSD1306_FONT font, char ch)
{
    uint32_t b;

    // Printable Characters : 32 - 126
    if(ch < 32 || ch > 126)
        return 0;

    // Check remaining space on current line
    if (SSD1306_WIDTH < (cursor.x + font.width) ||  SSD1306_HEIGHT < (cursor.y + font.height))
    {
        // Not enough space on current line
        return 0;
    }

    // Use the font to write
    for(int i = 0; i < font.height; i++)
    {
        b = font6x8.data[(ch - 32) * font.height + i];
        
        for(int j = 0; j < font.width; j++)
        {
            if((b << j) & 0x8000)
            {
                ssd1306_white_pixel(cursor.x + j, cursor.y + i);
            }
            else
            {
                ssd1306_black_pixel(cursor.x + j, cursor.y + i);
            }
        }
    }

    // The current space is now taken
    cursor.x += font.width;

    // Return written char for validation
    return ch;
}

/*
// Write full string to screen buffer
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color)
{
    // Write until null-byte
    while(*str)
    {
        if(ssd1306_WriteChar(*str, Font, color) != *str)
        {
            // Char could not be written
            return *str;
        }

        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

*/

void ssd1306_set_cursor(uint8_t x, uint8_t y)
{
    cursor.x = x;
    cursor.y = y;
}