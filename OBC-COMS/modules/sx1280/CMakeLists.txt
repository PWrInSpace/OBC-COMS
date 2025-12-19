# Modules/SX1280/CMakeLists.txt
add_library(SX1280_Lib STATIC 
    src/sx1280.c
    src/sx1280_hal_wrapper.c # Wrapper to isolate HAL from the driver
)
target_include_directories(SX1280_Lib PUBLIC inc)