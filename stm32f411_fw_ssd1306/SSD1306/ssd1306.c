/*
 * ssd1306.c
 *
 *  Created on: 2021. 6. 30.
 *      Author: mokhwasomssi
 */


#include "ssd1306.h"
#include <string.h> // memcpy


/* SSD1306 variable */

static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];
static SSD1306 ssd1306;


/* SSD1306 static function prototype */

static void reset();
static void ssd1306_write_command(uint8_t command);
static void ssd1306_write_data(uint8_t* buffer, uint16_t size);
static SSD1306_STATE ssd1306_fill_buffer(uint8_t* buf, uint32_t len);


// 0. Command Table for Charge Bump Setting

static void charge_bump_setting(uint8_t charge_bump);


// 1. Fundamental Command Table

static void set_contrast_control(uint8_t value);    // 256 constrast steps

static void set_entire_display_off();   // Resume to RAM content display (reset)
static void set_entire_display_on();

static void set_normal_display();       // 0x00 : BLACK, 0x01 : WHITE
static void set_inverse_display();      // 0x00 : WHITE, 0x01 : BLACK

static void set_display_off();          // sleep mode
static void set_display_on();           // normal mode


// 2. Scrolling Command Table


// 3. Addressing Setting Command Table

static void set_memory_addressing_mode(uint8_t mode);
static void set_lower_column_start_address(uint8_t addr);
static void set_higher_column_start_address(uint8_t addr);
static void set_page_start_address(uint8_t page);


// 4. Hardware Configuration (Panel resolution & layout related) Command Table

static void set_display_start_line(uint8_t start_line);     // 0x40 - 0x7F
static void set_segment_remap(uint8_t mapping);
static void set_multiplex_ratio(uint8_t mux);               // 15 - 63 (reset)
static void set_com_output_scan_direction(uint8_t mode); 
static void set_display_offset(uint8_t vertical_shift);     // 0 (reset) - 63
static void set_com_pins_hardware_config(uint8_t com_pin_config, uint8_t com_left_right_remap);


// 5. Timing & Driving Scheme Setting Command Table

static void set_display_clock_divide_ratio_and_osc_freq(uint8_t divide_ratio, uint8_t osc_freq);
static void set_pre_charge_period(uint8_t phase_1, uint8_t phase_2);
static void set_v_comh_deselect_level(uint8_t deselect_level);


/* SSD1306 global function */

void ssd1306_init()
{
    // Reset OLED
    reset();
    
    // Wait for the screen to boot
    HAL_Delay(100);

    // Init OLED
    set_display_off();

    set_memory_addressing_mode(PAGE_ADDRESSING);

    set_page_start_address(PAGE_0);

    set_com_output_scan_direction(NORMAL_MODE);

    set_lower_column_start_address(0x00);   // stating column is SEG0
    set_higher_column_start_address(0x10);

    set_display_start_line(0x40);

    set_contrast_control(CONTRAST_MAX); 

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










/* SSD1306 static function definition */


/* SSD1306 Interface */

static void ssd1306_write_command(uint8_t command)
{
    HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_SA_WRITE, SSD1306_CONTROL_BYTE_COMMAND, 1, &command, 1, 10);
}

static void ssd1306_write_data(uint8_t* buffer, uint16_t size)
{
    HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_SA_WRITE, SSD1306_CONTROL_BYTE_DATA, 1, buffer, size, 10);
}

static void reset()
{
    /* Nothing to do */
}

static SSD1306_STATE ssd1306_fill_buffer(uint8_t* buf, uint32_t len)
{
    SSD1306_STATE ret = SSD1306_ERR;

    if(len <= SSD1306_BUFFER_SIZE)
    {
        memcpy(ssd1306_buffer, buf, len);
        ret = SSD1306_OK;
    }

    return ret;
}


/* SSD1306 Command */

// 0. Command Table for Charge Bump Setting

static void charge_bump_setting(uint8_t charge_bump)
{
    ssd1306_write_command(CHARGE_BUMP_SETTING);
    ssd1306_write_command(charge_bump);
}


// 1. Fundamental Command Table

static void set_contrast_control(uint8_t value)
{
    ssd1306_write_command(SET_CONTRAST_CONTROL);
    ssd1306_write_command(value);
}

static void set_entire_display_off()
{
    ssd1306_write_command(ENTIRE_DISPLAY_OFF);
}

static void set_entire_display_on()
{
    ssd1306_write_command(ENTIRE_DISPLAY_ON);
}

static void set_normal_display()
{
    ssd1306_write_command(SET_NORMAL_DISPLAY);
}

static void set_inverse_display()
{
    ssd1306_write_command(SET_INVERSE_DISPLAY);
}

static void set_display_on()
{
    ssd1306_write_command(SET_DISPLAY_ON);
}

static void set_display_off()
{
    ssd1306_write_command(SET_DISPLAY_OFF);
}


// 3. Addressing Setting Command Table

static void set_memory_addressing_mode(uint8_t mode)
{
    ssd1306_write_command(SET_MEMORY_ADDRESSING_MODE);
    ssd1306_write_command(mode);
}

static void set_lower_column_start_address(uint8_t addr)
{
    ssd1306_write_command(addr);
}

static void set_higher_column_start_address(uint8_t addr)
{
    ssd1306_write_command(addr);
}

static void set_page_start_address(uint8_t page)  // 0xB0(page0) ~ 0xB7(page7)
{
    ssd1306_write_command(page);
}


// 4. Hardware Configuration (Panel resolution & layout related) Command Table

static void set_display_start_line(uint8_t start_line)
{
    ssd1306_write_command(start_line);
}

static void set_segment_remap(uint8_t mapping)
{
    ssd1306_write_command(mapping);
}

static void set_multiplex_ratio(uint8_t mux)
{
    ssd1306_write_command(SET_MULTIPLEX_RATIO);
    ssd1306_write_command(mux);
}

static void set_com_output_scan_direction(uint8_t mode)
{
    ssd1306_write_command(mode);
}

static void set_display_offset(uint8_t vertical_shift)
{
    ssd1306_write_command(SET_DISPLAY_OFFSET);
    ssd1306_write_command(vertical_shift);
}

static void set_com_pins_hardware_config(uint8_t com_pin_config, uint8_t com_left_right_remap)
{
    uint8_t buffer = com_pin_config | com_left_right_remap;

    ssd1306_write_command(SET_COM_PINS_HARDWARE_CONFIG);
    ssd1306_write_command(buffer);
}


// 5. Timing & Driving Scheme Setting Command Table

static void set_display_clock_divide_ratio_and_osc_freq(uint8_t divide_ratio, uint8_t osc_freq)
{
    uint8_t buffer = (osc_freq << 4) | divide_ratio;

    ssd1306_write_command(SET_DISPLAY_CLOCK_DIVIDE_RATIO_AND_OSC_FREQ);
    ssd1306_write_command(buffer);
}

static void set_pre_charge_period(uint8_t phase_1, uint8_t phase_2)
{
    uint8_t buffer = (phase_2 << 4) | phase_1;

    ssd1306_write_command(SET_PRE_CHARGE_PERIOD);
    ssd1306_write_command(buffer);
}

static void set_v_comh_deselect_level(uint8_t deselect_level)
{
    ssd1306_write_command(SET_V_COMH_DESELECT_LEVEL);
    ssd1306_write_command(deselect_level);
}
