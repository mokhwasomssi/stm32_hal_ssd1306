/*
 * ssd1306.c
 *
 *  Created on: 2021. 6. 30.
 *      Author: mokhwasomssi
 * 
 *       Table: SSD1306 Header
 *              SSD1306 Variable
 *              SSD1306 Write Function
 *              SSD1306 Command Function
 *  
 *                  0. Charge Bump Setting
 *                  1. Fundamental
 *                  2. Scrolling
 *                  3. Addressing Setting
 *                  4. Hardware Configuration
 *                  5. Timing & Driving Scheme
 * 
 *              SSD1306 Function
 * 
 *                  ssd1306_write_command
 *                  ssd1306_write_data
*                   ssd1306_init
 * 
 */



/* SSD1306 Header */ 
#include "ssd1306.h"
#include <string.h> // memcpy



/* SSD1306 Variable */

uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];
SSD1306 ssd1306;



/* SSD1306 function */

// I2C Write
void ssd1306_write_command(uint8_t command)
{
    HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_SA_WRITE, SSD1306_CONTROL_BYTE_COMMAND, 1, &command, 1, 10);
}

void ssd1306_write_data(uint8_t* buffer, uint16_t size)
{
    HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_SA_WRITE, SSD1306_CONTROL_BYTE_DATA, 1, buffer, size, 10);
}


// Charge Bump Setting Command
void charge_bump_setting(uint8_t charge_bump)
{
    ssd1306_write_command(CHARGE_BUMP_SETTING);
    ssd1306_write_command(charge_bump);
}


// Fundamental Command
void set_contrast_control(uint8_t value)
{
    ssd1306_write_command(SET_CONTRAST_CONTROL);
    ssd1306_write_command(value);
}

void set_entire_display_off()
{
    ssd1306_write_command(ENTIRE_DISPLAY_OFF);
}

void set_entire_display_on()
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


// Scrolling Command
// 안써용~


// Addressing Setting Command
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


// Hardware Configuration Command
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
    uint8_t buffer = 0x02 | com_pin_config | com_left_right_remap;

    ssd1306_write_command(SET_COM_PINS_HARDWARE_CONFIG);
    ssd1306_write_command(buffer);
}


// Timing & Driving Scheme Setting Command
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


// Initialize
void ssd1306_init()
{
    // Wait for the screen to boot
    HAL_Delay(100);

    // Init OLED
    set_display_off();

    set_memory_addressing_mode(PAGE_ADDRESSING);

    set_page_start_address_for_page_addressing_mode(PAGE_0);

    set_com_output_scan_direction(NORMAL_MODE);

    set_lower_column_start_address_for_page_addressing_mode(0x00);   // stating column is SEG0
    set_higher_column_start_address_for_page_addressing_mode(0x10);

    set_display_start_line(0x40);

    set_contrast_control(CONTRAST_RESET);

    set_segment_remap(MAP);

    set_normal_display();
    
    set_multiplex_ratio(MUX_RESET);

    set_entire_display_off();

    set_display_offset(VERTICAL_SHIFT_RESET);

    set_display_clock_divide_ratio_and_osc_freq(DIVIDE_RATIO_RESET, OSC_FREQ_RESET);

    set_pre_charge_period(PHASE_1_RESET, PHASE_2_RESET);

    set_com_pins_hardware_config(ALTERNATIVE_COM_PIN_CONFIG, DISABLE_COM_LEFT_RIGHT_REMAP);

    set_v_comh_deselect_level(DESELECT_LEVEL_2);
    

    charge_bump_setting(ENABLE_CHARGE_BUMP);

    set_display_on();
}
