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
 * @file    mmio.c
 * @version v1.0.0 -- CHANGELOG
 *                  | v1.0.0 - Added memory mapping function for GPIO access on
 *                  |           user space
 * 
 * @note    This library is written in C for Beagle Bone Black platform to map
 *          the GPIO for various user activities

 * @sa      doc/..Technical_Reference_Manual.pdf
 * 
 */

//===== INCLUDE ==============================================================//
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mmio.h"
//============================================================================//

//===== CONSTANTS AND VARIABLES ==============================================//
/*!
 * @brief Following are the addresses of memory mapped gpio
 * @sa    doc/..Technical_Reference_Manual -- Section 2. Memory Map
 */

static const uint32_t GPIO_LEN              = 4096;
static const uint32_t GPIO0_ADDR            = 0x44E07000;
static const uint32_t GPIO1_ADDR            = 0x4804C000;
static const uint32_t GPIO2_ADDR            = 0x481AC000;
static const uint32_t GPIO3_ADDR            = 0x481AF000;

// Memory access location
static const char DEV_MEM_LOC[] = "/dev/mem";

// Storing the GPIO base addresses
static const uint32_t gpio_addr[4] = {
    GPIO0_ADDR,
    GPIO1_ADDR,
    GPIO2_ADDR,
    GPIO3_ADDR
};

// Cache memory-mapped GPIO addresses
static volatile uint32_t *gpio_base[4] = { NULL };
//============================================================================//

//===== FUNCTION DEFINITION ==================================================//
int mmio_get_gpio(int base, int num, gpio_t *gpio) {
    // Input check for possible args error
    if(gpio == NULL) { return MMIO_ERR_ARG; }
    if(base < 0 || base > 3) { return MMIO_ERR_ARG; }
    if(num < 0 || num > 31) { return MMIO_ERR_ARG; }

    // Map GPIO memory if not already mapped
    if(gpio_base[base] == NULL) {
        int fd = open(DEV_MEM_LOC, O_RDWR, O_SYNC);
        if(fd == -1) { return MMIO_ERR_DEVMEM; }        // Try running as root     

        // Mapping GPIO memory location for user processing
        gpio_base[base] = (uint32_t *) mmap(
            NULL,
            GPIO_LEN,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            fd,
            gpio_addr[base]
        );

        // Clearing the cache if memory mapping failed
        if(gpio_base[base] == MAP_FAILED) {
            gpio_base[base] = NULL;
            return MMIO_ERR_MMAP;
        }
    }

    // Setting up the gpio field from memory
    memset(gpio, 0, sizeof(gpio));
    gpio->base = gpio_base[base];
    gpio->num = num;

    return MMIO_SUCCESS;
}
//============================================================================//
