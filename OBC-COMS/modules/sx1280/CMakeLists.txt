# Automatically find all .c files in the src directory
file(GLOB SX128x_SOURCES "src/*.c")

# Build Library
add_library(sx128x STATIC ${SX128x_SOURCES})

# Automatically find all headers in the inc directory
target_include_directories(sx128x PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link to CubeMX
target_link_libraries(sx128x stm32cubemx)