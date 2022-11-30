cmake_minimum_required(VERSION 3.10)

set(SOURCES
    dht11.c
    dht_common.c
)

add_library(DHT11 ${SOURCES})

target_include_directories(DHT11 PUBLIC .)

target_link_libraries(DHT11 PRIVATE MMIO)
