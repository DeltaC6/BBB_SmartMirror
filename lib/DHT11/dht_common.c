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
 * @file    dht_common.c
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
#include <time.h>
#include <sys/time.h>
#include <sched.h>
#include <string.h>
#include <errno.h>

#include "dht_common.h"
//============================================================================//

//===== CONSTANTS AND VARIABLES ==============================================//
//============================================================================//

//===== FUNCTION DEFINITION ==================================================//
void block_wait_ms(uint32_t ms) {
    // Setting delay time
    struct timeval deltaTime;
    deltaTime.tv_sec = ms / 1000;
    deltaTime.tv_usec = (ms % 1000) * 1000;

    // Getting current time
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    // Adding delay to current time to get new time
    struct timeval finalTime;
    timeradd(&currentTime, &deltaTime, &finalTime);

    // Blocking mode delay
    while(timercmp(&currentTime, &finalTime, <)) {
        gettimeofday(&currentTime, NULL);
    }
}

void sleep_ms(uint32_t ms) {
    struct timespec sleep;
    sleep.tv_sec = ms / 1000;
    sleep.tv_nsec = (ms % 1000) * 1000000L;
    while(clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep, &sleep) && errno == EINTR);
}

void set_max_priority(void) {
    struct sched_param sched;
    memset(&sched, 0, sizeof(sched));
    sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sched);
}

void set_default_priority(void) {
    struct sched_param sched;
    memset(&sched, 0, sizeof(sched));
    sched.sched_priority = 0;
    sched_setscheduler(0, SCHED_OTHER, &sched);
}
//============================================================================//
