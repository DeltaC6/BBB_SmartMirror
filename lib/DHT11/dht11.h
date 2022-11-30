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
 * @file    dht11.h
 * @version v1.0.0 -- CHANGELOG
 *                  | v1.0.0 - Added DHT sensor readout function
 * 
 * @note    This is a library written in standard C to interface DHT11 sensor
 *          on beagle bone black
 * 
 * @sa      doc/..Technical_Reference_Manual.pdf
 * 
 */

#ifndef __DHT11_H__
#define __DHT11_H__

//===== INCLUDE ==============================================================//
#include "dht_common.h"
//============================================================================//

//===== CONSTANTS AND VARIABLES ==============================================//
//============================================================================//

//===== FUNCTION DECLARATION =================================================//
/*!
 * @brief This function reads the DHT sensor for humidity and temperature values
 * @param [type] int - Type of sensor i.e. DHT11, DHT22 etc.
 * @param [base] int - Gpio base where the sensor is connected (0..3)
 * @param [num] int - GPIO pin number where the sensor is connected (0..31)
 * @param [humidity] float - Output of humidity value
 * @param [temperature] float - Output of temperature value
 * @return 0 if success else negative values for various possible errors
 * 
 * @note Some errors can be ignored and retried for e.g. in cases of timeout
 *       and checksum etc.
 */
int dht_read(int type, int base, int num, float *humidity, float *temperature);
//============================================================================//

#endif //__DHT11_H__
