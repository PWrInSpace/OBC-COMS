# Modules/SX1280/CMakeLists.txt
add_library(sd_card_Lib STATIC 
    src/sd_card.c
    )
target_include_directories(sd_card_Lib PUBLIC inc)

target_link_libraries(sd_card_Lib stm32cubemx)