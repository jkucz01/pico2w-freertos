cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico2_w)
set(PICO_PLATFORM rp2350)

set(PICO_SDK_PATH "~/pico-freertos/pico-sdk")
include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")

project(${EXEC_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 26)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

pico_sdk_init()

SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel" CACHE STRING "Course Common Lib")
SET(CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/config" CACHE STRING "Local Config")
message(STATUS "CONFIG_FILE_DIRECTORY: ${CONFIG_FILE_DIRECTORY}")

add_subdirectory(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/Community-Supported-Ports/GCC/RP2350_ARM_NTZ/ FreeRTOS-Kernel)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/lib/Pico-OLED-1-3)

add_executable(${EXEC_NAME} src/${EXEC_NAME}.c)

if(EXEC_NAME STREQUAL "ble-client")
    target_compile_definitions(${EXEC_NAME} PRIVATE RUNNING_AS_CLIENT=1)
endif()

target_compile_definitions(${EXEC_NAME} PRIVATE CYW43_USE_SPI=1)

target_link_libraries(${EXEC_NAME} 
    pico_stdlib
    pico_btstack_ble
    FreeRTOS-Kernel-Heap4
    pico_cyw43_arch_none
    pico_btstack_cyw43
    Pico-OLED-1-3
)

target_include_directories(${EXEC_NAME} PRIVATE 
    ${FREERTOS_KERNEL_PATH}/include
    ${CONFIG_FILE_DIRECTORY}
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${PICO_SDK_PATH}/lib/btstack/src
    ${PICO_SDK_PATH}/lib/btstack/platform/embedded
)

pico_add_extra_outputs(${EXEC_NAME})
pico_enable_stdio_usb(${EXEC_NAME} 1)
pico_enable_stdio_uart(${EXEC_NAME} 1)

install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${EXEC_NAME}.elf)")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/src/${EXEC_NAME}.uf2 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.elf)")

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.uf2
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
