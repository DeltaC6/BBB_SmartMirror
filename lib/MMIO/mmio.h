/***
 *      ___        ___       _   _                _ _            _           
 *     |  _|  /\  |_  |     | | | |              (_) |        /\| |/\        
 *     | |   /  \   | |_   _| |_| |__   ___  _ __ _| |_ _   _ \ ` ' / ______ 
 *     | |  / /\ \  | | | | | __| '_ \ / _ \| '__| | __| | | |_     _|______|
 *     | | / ____ \ | | |_| | |_| | | | (_) | |  | | |_| |_| |/ , . \        
 *     | |/_/    \_\| |\__,_|\__|_| |_|\___/|_|  |_|\__|\__, |\/|_|\/        
 *     |___|      |___|                                  __/ |               
 *                                                      |___/                
 *
 * @author  Syed Asad Amin
 * @date    Nov 30th, 2022
 * @file    mmio.h
 * @version v1.0.0 -- CHANGELOG
 *                  | v1.0.0 - Added memory mapping function for GPIO access on
 *                  |           user space
 * 
 * @note    This library is written in C for Beagle Bone Black platform to map
 *          the GPIO for various user activities

 * @sa      doc/..Technical_Reference_Manual.pdf
 * 
 */

#ifndef __MMIO_H__
#define __MMIO_H__

//===== INCLUDE ==============================================================//
#include <stdint.h>
//============================================================================//

//===== TYPES, CONSTANTS AND VARIABLES =======================================//
typedef enum GPIO_LVL {
    LOW,
    HIGH,
} gpio_lvl_e;

typedef enum MMIO_ERR {
    MMIO_ERR_MMAP                               = -3,
    MMIO_ERR_DEVMEM                             = -2,
    MMIO_ERR_ARG                                = -1,
    MMIO_SUCCESS                                = 0
} mmio_err_e;

static const uint32_t MMIO_OE_ADDR              = 0x134;
static const uint32_t MMIO_IO_DATAIN            = 0x138;
static const uint32_t MMIO_IO_DATAOUT           = 0x13C;
static const uint32_t MMIO_IO_CLR_DATAOUT       = 0x190;
static const uint32_t MMIO_IO_SET_DATAOUT       = 0x194;

typedef struct GPIO {
    volatile uint32_t *base;
    uint8_t num;
} gpio_t;
//============================================================================//

//===== FUNCTION DECLARATION =================================================//

/*!
 * @brief Get the gpio form memory and map it in user space
 * @param [base] int - Gpio base (0..3)
 * @param [num] int - GPIO number (0..31)
 * @param [gpio_t] gpio - Output of mapped GPIO
 * @return 0 if success else negative values for various possible errors
 */
int mmio_get_gpio(int base, int num, gpio_t *gpio);

/*!
 * @brief Sets the GPIO as input mode
 * @param [gpio_t] gpio - GPIO to be set as input
 * @return None
 */
static inline void mmio_set_input(gpio_t gpio) {
    gpio.base[MMIO_OE_ADDR / 4] |= (1 << gpio.num);
}

/*!
 * @brief Sets the GPIO as output mode
 * @param [gpio_t] gpio - GPIO to be set as output
 * @return None
 */
static inline void mmio_set_output(gpio_t gpio) {
    gpio.base[MMIO_OE_ADDR / 4] &= (0xFFFFFFFF ^ (1 << gpio.num));
}

/*!
 * @brief Sets the GPIO level to low
 * @param [gpio_t] gpio - GPIO to be set at low level
 * @return None
 */
static inline void mmio_set_low(gpio_t gpio) {
    gpio.base[MMIO_IO_CLR_DATAOUT / 4] = 1 << gpio.num;
}

/*!
 * @brief Sets the GPIO level to high
 * @param [gpio_t] gpio - GPIO to be set at high level
 * @return None
 */
static inline void mmio_set_high(gpio_t gpio) {
    gpio.base[MMIO_IO_SET_DATAOUT / 4] = 1 << gpio.num;
}

/*!
 * @brief Sets the GPIO level
 * @param [gpio_t] gpio - GPIO to be set
 * @param [lvl] int - Level of GPIO
 * @return None
 */
static inline void mmio_set_level(gpio_t gpio, int lvl) {
    if(lvl == 0) {
        mmio_set_low(gpio);
    } else {
        mmio_set_high(gpio);
    }
}

/*!
 * @brief Get the input state of the GPIO
 * @param [gpio_t] gpio - GPIO to read
 * @return [uint32_t] Level of gpio
 */
static inline uint32_t mmio_input(gpio_t gpio) {
    return gpio.base[MMIO_IO_DATAIN / 4] & (1 << gpio.num);
}
//============================================================================//

#endif //__MMIO_H__
