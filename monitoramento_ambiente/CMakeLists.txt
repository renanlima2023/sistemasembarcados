# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.10)

# Configurações de padrões do projeto
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Definir o tipo da placa
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Incluir o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

# Inicializar o SDK do Raspberry Pi Pico
pico_sdk_init()

# Definir o projeto
project(monitoramento_ambiente C CXX ASM)

# Adicionar executável
add_executable(monitoramento_ambiente monitoramento_ambiente.c ssd1306.c)

# Definir nome e versão do programa
pico_set_program_name(monitoramento_ambiente "monitoramento_ambiente")
pico_set_program_version(monitoramento_ambiente "0.1")

# Ativar saída por UART/USB
pico_enable_stdio_uart(monitoramento_ambiente 1)
pico_enable_stdio_usb(monitoramento_ambiente 1)

# Adicionar a biblioteca padrão
target_link_libraries(monitoramento_ambiente pico_stdlib)

# Incluir diretórios necessários
target_include_directories(monitoramento_ambiente PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Adicionar bibliotecas adicionais, como hardware I2C, PWM e SSD1306
target_link_libraries(monitoramento_ambiente pico_stdlib hardware_i2c hardware_pwm pico-ssd1306)

# Gerar saídas extras (como .uf2)
pico_add_extra_outputs(monitoramento_ambiente)
