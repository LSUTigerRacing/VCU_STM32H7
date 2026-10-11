add_library(seggerRTT
    SEGGER_RTT_printf.c
    SEGGER_RTT.c
    SEGGER_RTT_ASM_ARMv7M.S)

target_include_directories(seggerRTT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})