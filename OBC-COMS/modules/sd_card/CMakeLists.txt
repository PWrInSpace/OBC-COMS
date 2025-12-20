# 1. Automatically find all .c files in the src directory
file(GLOB SD_CARD_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# 2. Build the Library target
add_library(sd_card_Lib STATIC ${SD_CARD_SOURCES})

# 3. Automatically include the inc and src folders for headers
target_include_directories(sd_card_Lib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# 4. Link to CubeMX for HAL and main.h access
target_link_libraries(sd_card_Lib stm32cubemx)