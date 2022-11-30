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
 * @file    dht11.c
 * @version v1.0.0 -- CHANGELOG
 *                  | v1.0.0 - Added DHT sensor readout function
 * 
 * @note    This is a library written in standard C to interface DHT11 sensor
 *          on beagle bone black
 * 
 * @sa      doc/..Technical_Reference_Manual.pdf
 * 
 */

//===== INCLUDE ==============================================================//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dht11.h"
#include "mmio.h"
//============================================================================//

//===== CONSTANTS AND VARIABLES ==============================================//
static const uint32_t DHT_MAXCOUNT  = 32000;
static const uint32_t DHT_PULSES    = 41;
//============================================================================//

//===== FUNCTION DEFINITION ==================================================//
int dht_read(int type, int base, int num, float *humidity, float *temperature) {
    // Parameters validity checks
    if(humidity == NULL || temperature == NULL) { return DHT_ERR_ARGUMENT; }
    *humidity = 0.0f;
    *temperature = 0.0f;

    int pulseCounts[DHT_PULSES * 2];
    memset(pulseCounts, 0, sizeof(pulseCounts));

    // Get gpio pin and set as output
    gpio_t pin;
    if(mmio_get_gpio(base, num, &pin) < 0) { return DHT_ERR_GPIO; }
    mmio_set_output(pin);

    // Making sure process becomes faster and avoid kernel context switching
    set_max_priority();

    // Sending pulse to DHT sensor to let it know to start transmission of
    // 40 data bits
    mmio_set_high(pin);
    sleep_ms(500);\
    mmio_set_low(pin);
    block_wait_ms(20);

    // Set gpio pin as input
    mmio_set_input(pin);

    // Wait for DHT to pull pin low.
    uint32_t count = 0;
    while(mmio_input(pin)) {
        if(++count >= DHT_MAXCOUNT) {
            set_default_priority();
            return DHT_ERR_TIMEOUT;
        }
    }

    // Record pulse
    for(int i = 0; i < DHT_PULSES * 2; i+=2) {
        // Check for pin low and store it
        while(!mmio_input(pin)) {
            if(++pulseCounts[i] >= DHT_MAXCOUNT) {
                set_default_priority();
                return DHT_ERR_TIMEOUT;
            }
        }

        // Check for pin high and store it
        while(mmio_input(pin)) {
            if(++pulseCounts[i+1] >= DHT_MAXCOUNT) {
                set_default_priority();
                return DHT_ERR_TIMEOUT;
            }
        }
    }

    // Set back to default priority since critical task section is complete
    set_default_priority();

    // Get the average low pulse width to use as 50 microsec reference threshold
    uint32_t threshold = 0;
    for(int i = 2; i < DHT_PULSES * 2; i+=2) {
        threshold += pulseCounts[i];
    }
    threshold /= DHT_PULSES - 1;

    // Interpreting pulses
    uint8_t data[5] = { 0 };
    for(int i = 3; i < DHT_PULSES * 2; i+=2) {
        int index = (i-3) / 16;
        data[index] <<= 1;
        if(pulseCounts[i] >= threshold) {
            data[index] |= 1;
        }
    }

    // Debugging only
    fprintf(stdout, "Data: 0x%x 0x%x 0x%x 0x%x 0x%x\n", data[0], data[1], data[2], data[3], data[4]);

    // Verify CSUM
    if(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        if(type == DHT11) {
            *humidity = (float) data[0];
            *temperature = (float) data[2];
        } else if(type == DHT22) {
            *humidity = (data[0] * 256 + data[1]) / 10.0f;
            *temperature = ((data[2] * 0x7F) * 256 + data[3]) / 10.0f;
            if(data[2] & 0x80) {
                *temperature *= -1.0f;
            }
        }
        return DHT_SUCCESS;
    } else {
        return DHT_ERR_CHECKSUM;
    }
}
//============================================================================//
