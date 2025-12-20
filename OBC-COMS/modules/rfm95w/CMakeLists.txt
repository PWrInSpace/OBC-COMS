# Modules/SX1280/CMakeLists.txt
add_library(rfm95w_Lib STATIC 
    src/rfm95w.c
    src/rfm95w_hal_wrapper.c # Wrapper to isolate HAL from the driver
)
target_include_directories(rfm95w_Lib PUBLIC inc)
target_link_libraries(rfm95w_Lib stm32cubemx)