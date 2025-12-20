# Modules/SX1280/CMakeLists.txt
add_library(max2201_Lib STATIC 
    src/max2201.c
    src/max2201_hal_wrapper.c # Wrapper to isolate HAL from the driver
)
target_include_directories(max2201_Lib PUBLIC inc)
target_link_libraries(max2201_Lib stm32cubemx)