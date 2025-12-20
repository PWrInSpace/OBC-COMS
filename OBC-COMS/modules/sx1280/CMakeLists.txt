# modules/sx1280/CMakeLists.txt

# Set source files with the correct relative path
set(SX128x_SOURCES
    src/sx1280.c
    src/sx1280-hal.c
    src/sx1280-radio.c
)

# Build Library
add_library(sx128x STATIC ${SX128x_SOURCES})

# Ensure headers are found in the 'inc' folder
target_include_directories(sx128x PUBLIC inc)

# Link to CubeMX for HAL definitions
target_link_libraries(sx128x stm32cubemx)