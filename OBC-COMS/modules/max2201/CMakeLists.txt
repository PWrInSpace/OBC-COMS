# 1. Automatically find all .c files in the src directory
file(GLOB MAX2201_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# 2. Build the Library target
add_library(max2201_Lib STATIC ${MAX2201_SOURCES})

# 3. Automatically include the inc and src folders for headers5
target_include_directories(max2201_Lib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# 4. Link to stm32cubemx for HAL and main.h access
target_link_libraries(max2201_Lib stm32cubemx)