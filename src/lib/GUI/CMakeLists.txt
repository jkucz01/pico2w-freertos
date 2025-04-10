add_library(GUI
    GUI_Paint.c
)

target_include_directories(GUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GUI PUBLIC Config)