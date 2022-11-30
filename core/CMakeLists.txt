cmake_minimum_required(VERSION 3.10)

set(SOURCES
    main.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall               # Enable all warnings
    -Wextra             # Enable extra warnings
    -Wpedantic          # Enable pedantic warnings
    -Wno-unused         # Disable unused parametrs and functions
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>: -Og -g3 -ggdb>
    $<$<CONFIG:Release>: -O0 -g0>

    # Add other compile options below
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    DHT11
)
