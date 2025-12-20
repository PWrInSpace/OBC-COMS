# 1. Automatically find all .c files in the src directory
file(GLOB RFM95W_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# 2. Build the Library target
add_library(rfm95w_Lib STATIC ${RFM95W_SOURCES})

# 3. Automatically include the inc and src folders for headers
target_include_directories(rfm95w_Lib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# 4. Link to stm32cubemx for HAL and main.h access
target_link_libraries(rfm95w_Lib stm32cubemx)