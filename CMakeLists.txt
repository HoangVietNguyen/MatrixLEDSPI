cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME MatrixLEDSPI)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        Core/Src/tetris_factory/tetris_factory.h
        Core/Src/tetris_factory/tetris_factory.c
        Core/Src/ICMax7219_driver/ICMax7219_driver.c
        Core/Src/ICMax7219_driver/ICMax7219_driver.h
        Core/Src/spi_driver/spi_driver.c
        Core/Src/spi_driver/spi_driver.h
        Core/Src/gpio_driver/gpio_driver.c
        Core/Src/gpio_driver/gpio_driver.h
        Core/Src/rcc_driver/rcc_driver.c
        Core/Src/rcc_driver/rcc_driver.h
        Core/Src/tim_driver/tim_driver.c
        Core/Src/tim_driver/tim_driver.h
        Core/Src/generate_piece_driver/pieces.c
        Core/Src/generate_piece_driver/pieces.h
        Core/Src/lcd_i2c_driver/lcd_i2c.c
        Core/Src/lcd_i2c_driver/lcd_i2c.h
        Core/Src/dht11_driver/dht11.c
        Core/Src/dht11_driver/dht11.h
        Core/Src/adc_driver/adc.c
        Core/Src/adc_driver/adc.h
        Core/Src/usart_driver/usart.c
        Core/Src/usart_driver/usart.h
        Core/Src/lora_driver_send/lora_send.c
        Core/Src/lora_driver_send/lora_send.h
        Core/Src/lora_driver_receive/lora_receive.c
        Core/Src/lora_driver_receive/lora_receive.h
        Core/Src/matrix_stack/matrix_stack.h
        Core/Src/matrix_stack/matrix_stack.c
        Core/Src/rcc_driver/rcc_driver.c
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
