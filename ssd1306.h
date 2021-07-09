/*
 * ssd1306.h
 *
 *  Created on: 2021. 6. 30.
 *      Author: mokhwasomssi
 * 
 */


#ifndef __SSD1306_H__
#define __SSD1306_H__


#include "i2c.h" // header from stm32cubemx code generate


/* SSD1306 Interface */ 

// I2C bus data format
// Start - Slave Address(R/W) - Control byte(D/C) - Data byte - Stop 

#define SSD1306_I2C                     (&hi2c1)
#define SSD1306_I2C_SA                  0x3C    // Slave address

#define SSD1306_I2C_SA_READ             (SSD1306_I2C_SA << 1 | 1)
#define SSD1306_I2C_SA_WRITE            (SSD1306_I2C_SA << 1 | 0)

#define SSD1306_CONTROL_BYTE_DATA        0x40
#define SSD1306_CONTROL_BYTE_COMMAND     0x00


/* SSD1306 Constant */

#define SSD1306_WIDTH           128
#define SSD1306_HEIGHT          64

#define SSD1306_BUFFER_SIZE     SSD1306_WIDTH * SSD1306_HEIGHT / 8


/* SSD1306 Enumeration */

typedef enum
{
    BLACK = 0x00,
    WHITE = 0x01

} SSD1306_COLOR;

typedef enum
{
    SSD1306_OK = 0x00,
    SSD1306_ERR = 0x01,

} SSD1306_STATE;



/* SSD1306 Struct */
typedef struct
{
    uint16_t current_x;
    uint16_t current_y;

    uint8_t inverted;
    
    uint8_t initialized;
    uint8_t display_on;

} SSD1306;

typedef struct
{
    uint8_t x;
    uint8_t y;

} SSD1306_VERTEX;


/* Charge Bump Setting Command */
#define CHARGE_BUMP_SETTING             0x8D   

/* Fundamental Command */
#define SET_CONTRAST_CONTROL            0x81   
#define ENTIRE_DISPLAY_OFF              0xA4    
#define ENTIRE_DISPLAY_ON               0xA5
#define SET_NORMAL_DISPLAY              0xA6   
#define SET_INVERSE_DISPLAY             0xA7   
#define SET_DISPLAY_OFF                 0xAE   
#define SET_DISPLAY_ON                  0xAF  

/* Scrolling Command */
#define CONTINUMOUS_HORIZONTAL_SCROLL_SETUP
#define CONTINUOUS_VERTICAL_AND_HORIZONTAL_SCROLL_SETUP
#define DEACTIVATE_SCROLL                                           0x2E
#define ACTIVATE_SCROLL                                             0x2F
#define SET_VERTICAL_SCROLL_AREA                                    0xA3

/* Addrssing Setting Command */
#define SET_LOWER_COLUMN_START_ADDRESS_FOR_PAGE_ADDRESSING_MODE  
#define SET_HIGHER_COLUMN_START_ADDRESS_FOR_PAGE_ADDRESSING_MODE 
#define SET_MEMORY_ADDRESSING_MODE                                  0x20
#define SET_COLUMN_ADDRESS                                          0x21
#define SET_PAGE_ADDRESS                                            0x22
#define SET_PAGE_START_ADDRESS_FOR_PAGE_ADDRESSING_MODE        

/* Hardware Configuration Command */
#define SET_DISPLAY_START_LINE        
#define SET_SEGMENT_REMAP              
#define SET_MULTIPLEX_RATIO                                         0xA8
#define SET_COM_OUPUT_SCAN_DIRECTION  
#define SET_DISPLAY_OFFSET                                          0xD3
#define SET_COM_PINS_HARDWARE_CONFIG                                0xDA

/* Timing & Driving Scheme Setting Command */
#define SET_DISPLAY_CLOCK_DIVIDE_RATIO_AND_OSC_FREQ                 0xD5
#define SET_PRE_CHARGE_PERIOD                                       0xD9
#define SET_V_COMH_DESELECT_LEVEL                                   0xDB


/* I2C write */
void ssd1306_write_command(uint8_t command);
void ssd1306_write_data(uint8_t* buffer, uint16_t size);


/* Charge Bump Setting */

// @param : 0x10(disable, reset), 0x14(enable)
// Note. if enable charge pump
// 0x8D : Charge Pump Setting
// 0x14 : Enable Charge Pump
// 0xAF : Display ON
void charge_bump_setting(uint8_t charge_bump); 


/* Fundamental */

// @param : 0 - 255, 127(reset)
void set_contrast_control(uint8_t value);

void set_entire_display_off();              // Resume to RAM content display(reset)
void set_entire_display_on();    
void set_normal_display();                  // 0x00 : BLACK, 0x01 : WHITE
void set_inverse_display();                 // 0x00 : WHITE, 0x01 : BLACK
void set_display_off();                     // sleep mode (reset)
void set_display_on();                      // normal mode


/* Scrolling Command */
// 안써용~


/* Addressing Setting */

// @param : 0x00 - 0x0F
void set_lower_column_start_address_for_page_addressing_mode(uint8_t addr);

// @param : 0x10 - 0x1F
void set_higher_column_start_address_for_page_addressing_mode(uint8_t addr);

// @param : 0(horizontal). 1(vertical), 2(page, reset)
void set_memory_addressing_mode(uint8_t mode);

// @param : 0(reset) - 127
// @param : 0 - 127(reset)
void set_column_address(uint8_t start, uint8_t end);

// @param : 0(reset) - 7
// @param : 0 - 7(reset)
void set_page_address(uint8_t start, uint8_t end);

// @param : 0xB0 - 0xB7
void set_page_start_address_for_page_addressing_mode(uint8_t page);


/* Hardware Configuration */

// @param : 0x40(reset) - 0x7F
void set_display_start_line(uint8_t start_line);

// @param : 0xA0(map, reset), 0xA1(remap)
void set_segment_remap(uint8_t mapping);

// @param : 15 - 63(reset)
void set_multiplex_ratio(uint8_t mux);

// @param : 0xC0(normal mode, reset), 0xC8(remapped mode)
void set_com_output_scan_direction(uint8_t mode);

// @param : 0(reset) - 63
void set_display_offset(uint8_t vertical_shift);     

// @param : 0x00(sequential), 0x10(alternative, reset)
// @param : 0x00(disable), 0x20(enable)
void set_com_pins_hardware_config(uint8_t com_pin_config, uint8_t com_left_right_remap);


/* Timing & Driving Scheme Setting */

// @param : 0(reset) - 15
// @param : 0b0000 - 0b1111, 0b1000(reset)
void set_display_clock_divide_ratio_and_osc_freq(uint8_t divide_ratio, uint8_t osc_freq);   

// @param : 0x01 - 0x15, 0x02(reset)
// @param : 0x01 - 0x15, 0x02(reset)
void set_pre_charge_period(uint8_t phase_1, uint8_t phase_2);   

// @param : 0x00, 0x20(reset), 0x30
void set_v_comh_deselect_level(uint8_t deselect_level);


/* Initialize */
void ssd1306_init();


#endif /* __SSD1306_H__ */