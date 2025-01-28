# Minimum CMake version
cmake_minimum_required(VERSION 3.13)

# Define C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define the Raspberry Pi Pico SDK path if it's not set
# Set the path to your Pico SDK here if it's not installed via environment
if(NOT DEFINED PICO_SDK_PATH)
    set(PICO_SDK_PATH "C:/Users/renan/pico-sdk")  # Ajuste este caminho conforme necessário
endif()

# Import the Pico SDK
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Define the project name
project(animacao C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Set the board type
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Add the executable. Make sure the correct source files are included
add_executable(animacao animacao.c neopixel.c buzzer.c)

# Set the program name and version
pico_set_program_name(animacao "animacao")
pico_set_program_version(animacao "0.1")

# Enable output over UART/USB
pico_enable_stdio_uart(animacao 1)
pico_enable_stdio_usb(animacao 1)

# Add the standard library and hardware libraries to the build
target_link_libraries(animacao
    pico_stdlib
    hardware_pio
    hardware_adc
    pico_bootrom
    hardware_pwm
)

# Include directories for any extra code
target_include_directories(animacao PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/lib/ws2812  # Verifique se este diretório existe
)

# Add any user-requested libraries (if needed)
target_link_libraries(animacao)

# Generate the PIO header for the ws2818b.pio file
pico_generate_pio_header(animacao ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)

# Add extra outputs (e.g., UF2 file)
pico_add_extra_outputs(animacao)
