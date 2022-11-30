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
 * @file    dht_common.h
 * @version v1.0.0 -- CHANGELOG
 *                  | v1.0.0 - Added DHT sensor readout function
 * 
 * @note    This is a library written in standard C to interface DHT11 sensor
 *          on beagle bone black
 * 
 * @sa      doc/..Technical_Reference_Manual.pdf
 * 
 */

#ifndef __DHT_COMMON_H__
#define __DHT_COMMON_H__

//===== INCLUDE ==============================================================//
#include <stdint.h>

//===== TYPES, CONSTANTS AND VARIABLES =======================================//
typedef enum DHT_SENS_TYPE {
    DHT11                   = 11,
    DHT22                   = 22
} dht_type_e;

typedef enum DHT_ERR {
    DHT_ERR_TIMEOUT         = -4,
    DHT_ERR_CHECKSUM        = -3,
    DHT_ERR_ARGUMENT        = -2,
    DHT_ERR_GPIO            = -1,
    DHT_SUCCESS             = 0
} dht_err_e;
//============================================================================//

//===== FUNCTION DECLARATION =================================================//
/*!
 * @brief Blocking mode delay
 * @param [ms] uint32_t - Delay time in milliseconds
 * @return None
 */
void block_wait_ms(uint32_t ms);

/*!
 * @brief Sleep mode delay
 * @param [ms] uint32_t - Delay time in milliseconds
 * @return None
 */
void sleep_ms(uint32_t ms);

/*!
 * @brief Sets FIFO schedular priority to max to avoid kernel context switching
 * @param None
 * @return None
 */
void set_max_priority(void);

/*!
 * @brief Sets the default priority statues of schedular
 * @param None
 * @return None
 */
void set_default_priority(void);
//============================================================================//

#endif // __DHT_COMMON_H__
