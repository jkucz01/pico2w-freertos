add_library(OLED_Helper OLED.c)
target_include_directories(OLED_Helper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(OLED_Helper PUBLIC OLED GUI Fonts Config)
