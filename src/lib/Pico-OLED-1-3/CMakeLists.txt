# src/lib/Pico-OLED-1-3/CMakeLists.txt

add_library(Pico-OLED-1-3 STATIC
    DEV_Config.c
    GUI_Paint.c
    OLED_1in3_c.c
    OLED.c
    font12.c
)

target_include_directories(Pico-OLED-1-3 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Pico-OLED-1-3 PUBLIC pico_stdlib hardware_spi hardware_i2c hardware_pwm hardware_adc)