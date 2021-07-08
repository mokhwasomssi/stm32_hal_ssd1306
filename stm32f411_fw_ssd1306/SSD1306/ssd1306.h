/*
 * ssd1306.h
 *
 *  Created on: 2021. 6. 30.
 *      Author: mokhwasomssi
 */


#ifndef __SSD1306_H__
#define __SSD1306_H__

 
#include "i2c.h" // header from stm32cubemx code generate


/* SSD1306 Interface */ 

// I2C bus data format
// Start - Slave Address(R/W) - Control byte(D/C) - Data byte - Stop 

#define SSD1306_I2C                (&hi2c1)
#define SSD1306_I2C_SA             (0x3C)    // Slave address

#define SSD1306_I2C_SA_READ        (SSD1306_I2C_SA << 1 | 1)
#define SSD1306_I2C_SA_WRITE       (SSD1306_I2C_SA << 1 | 0)

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

/* SSD1306 global function */

void ssd1306_init();


/* SSD1306 Command */

// 0. Command Table for Charge Bump Setting

// Note. if enable charge pump
// 8Dh : Charge Pump Setting
// 14h : Enable Charge Pump
// AFh : Display ON

#define CHARGE_BUMP_SETTING         0x8D
#ifdef CHARGE_BUMP_SETTING
    // A[7:0]
    #define DISABLE_CHARGE_BUMP     0x10    // reset
    #define ENABLE_CHARGE_BUMP      0x14    
#endif


// 1. Fundamental Command Table


#define SET_CONTRAST_CONTROL    0x81   
#ifdef SET_CONTRAST_CONTROL
    // A[7:0]
    #define CONTRAST_STEP       256     
    #define CONTRAST_RESET      0x7F    // reset
    #define CONTRAST_MAX        0xFF
#endif


#define ENTIRE_DISPLAY_OFF      0xA4    // Resume to RAM content display (reset)
#define ENTIRE_DISPLAY_ON       0xA5


#define SET_NORMAL_DISPLAY      0xA6    // 0x00 : BLACK, 0x01 : WHITE
#define SET_INVERSE_DISPLAY     0xA7    // 0x00 : WHITE, 0x01 : BLACK


#define SET_DISPLAY_OFF         0xAE    // sleep mode
#define SET_DISPLAY_ON          0xAF    // normal mode


// 2. Scrolling Command Table


#define CONTINUMOUS_HORIZONTAL_SCROLL_SETUP
#ifdef CONTINUMOUS_HORIZONTAL_SCROLL_SETUP
    #define RIGHT                       0x26
    #define LEFT                        0x27

    // A[7:0]
    #define DUMMY_BYTE                  0x00

    // B[2:0]
    #define START_PAGE                  // 0 - 7
    
    // C[2:0]
    #define TIME_INTERVAL

    // D[2:0]
    #define END_PAGE                    // 0 - 7
#endif


#define CONTINUOUS_VERTICAL_AND_HORIZONTAL_SCROLL_SETUP
#ifdef CONTINUOUS_VERTICAL_AND_HORIZONTAL_SCROLL_SETUP
    // Horizontal scroll by 1 column
    #define VERTICAL_RIGHT_HORIZONTAL   0x29
    #define VERTICAL_LEFT_HORIZONTAL    0x2A

    // A[7:0]
    #define DUMMY_BYTE                  0x00

    // B[2:0]
    #define START_PAGE                  // 0 - 7
    
    // C[2:0]
    #define TIME_INTERVAL

    // D[2:0]
    #define END_PAGE                    // 0 - 7

    // E[5:0]
    #define VERTICAL_OFFSET             // 0 - 63
#endif


#define DEACTIVATE_SCROLL               0x2E
#define ACTIVATE_SCROLL                 0x2F


#define SET_VERTICAL_SCROLL_AREA        0xA3
#ifdef SET_VERTICAL_SCROLL_AREA
    // A[5:0]
    #define TOP_FIXED_AREA

    // B[6:0]
    #define SCROLL_AREA
#endif


// 3. Addressing Setting Command Table


#define SET_MEMORY_ADDRESSING_MODE  0x20
#ifdef SET_MEMORY_ADDRESSING_MODE
    // A[1:0]
    #define PAGE_ADDRESSING         0b10    // reset
    #define HORIZONTAL_ADDRESSING   0b00
    #define VERTICAL_ADDRESSING     0b01
#endif


/* only for page addressing mode*/
#define SET_LOWER_COLUMN_START_ADDRESS  // 0x00 ~ 0x0F
#define SET_HIGHER_COLUMN_START_ADDRESS // 0x10 ~ 0x1F


#define SET_PAGE_START_ADDRESS
#ifdef SET_PAGE_START_ADDRESS
    #define PAGE_0                   0xB0
    #define PAGE_1                   0xB1
    #define PAGE_2                   0xB2
    #define PAGE_3                   0xB3
    #define PAGE_4                   0xB4
    #define PAGE_5                   0xB5
    #define PAGE_6                   0xB6
    #define PAGE_7                   0xB7
#endif


/* only for Horizontal or vertical addressing mode */
#define SET_COLUMN_ADDRESS          0x21
#ifdef SET_COLUMN_ADDRESS
    // A[6:0]
    #define START                   // 0(reset) - 127

    // B[6:0]
    #define END                     // 0 - 127(reset)
#endif


#define SET_PAGE_ADDRESS            0x22
#ifdef SET_PAGE_ADDRESS
    // A[2:0]
    #define START                   // 0(reset) - 7
    
    // B[2:0]
    #define END                     // 0 - 7(reset)
#endif


// 4. Hardware Configuration (Panel resolution & layout related) Command Table

#define SET_DISPLAY_START_LINE              // 0x40 - 0x7F


#define SET_SEGMENT_REMAP
#ifdef SET_SEGMENT_REMAP
    #define MAP                             0xA0    // reset
    #define REMAP                           0xA1
#endif


#define SET_MULTIPLEX_RATIO                 0xA8
#ifdef SET_MULTIPLEX_RATIO   
    // A[5:0]
    #define MUX                             // 15 - 63 (reset)
    #define MUX_RESET                       0x3F    // 63
#endif


#define SET_COM_OUTPUT_SCAN_DIRECTION
#ifdef SET_COM_OUTPUT_SCAN_DIRECTION               
    #define NORMAL_MODE                     0xC0    
    #define REMAPPED_MODE                   0xC8    // Vertically flipped
#endif


#define SET_DISPLAY_OFFSET                  0xD3
#ifdef SET_DISPLAY_OFFSET
    // A[5:0]
    #define VERTICAL_SHIFT                  // 0 (reset) - 63
    #define VERTICAL_SHIFT_RESET            0x00
#endif


#define SET_COM_PINS_HARDWARE_CONFIG        0xDA
#ifdef SET_COM_PINS_HARDWARE_CONFIG
    // A[4]
    #define ALTERNATIVE_COM_PIN_CONFIG      0x12    // reset
    #define SEQUENTIAL_COM_PIN_CONFIG       0x02

    // A[5]
    #define DISABLE_COM_LEFT_RIGHT_REMAP    0x02    // reset
    #define ENABLE_COM_LEFT_RIGHT_REMAP     0x22
#endif


// 5. Timing & Driving Scheme Setting Command Table


#define SET_DISPLAY_CLOCK_DIVIDE_RATIO_AND_OSC_FREQ     0xD5
#ifdef SET_DISPLAY_CLOCK_DIVIDE_RATIO_AND_OSC_FREQ
    // A[3:0]
    #define DIVIDE_RATIO            // 0 (reset) - 15
    #define DIVIDE_RATIO_RESET      0x00

    // A[7:4]
    #define OSC_FREQ                // 0b1000 (reset), 0b0000 - 0b1111, 
    #define OSC_FREQ_RESET          0x08
#endif


#define SET_PRE_CHARGE_PERIOD       0xD9
#ifdef SET_PRE_CHARGE_PERIOD
    // A[3:0]
    #define PHASE_1                 // 0x02 (reset), 0x01 - 0x15
    #define PHASE_1_RESET           0x02

    // A[7:4]
    #define PHASE_2                 // 0x02 (reset), 0x01 - 0x15
    #define PHASE_2_RESET           0x02
#endif


#define SET_V_COMH_DESELECT_LEVEL   0xDB
#ifdef SET_V_COMH_DESELECT_LEVEL
    // A[6:4]
    #define DESELECT_LEVEL_1        0x00    // 0.65 * Vcc
    #define DESELECT_LEVEL_2        0x20    // 0.77 * Vcc (reset)
    #define DESELECT_LEVEL_3        0x30    // 0x83 * Vcc
#endif


#define NOP                         0xE3    // No operation


#endif /* __SSD1306_H__ */