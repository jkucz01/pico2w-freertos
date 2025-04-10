add_library(OLED
    OLED_1in3_c.c
)

target_include_directories(OLED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(OLED PUBLIC Config)