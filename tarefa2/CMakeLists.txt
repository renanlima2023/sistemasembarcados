# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

# Definir os padrões C/C++
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializar o pico_sdk a partir do local instalado
# (Note que isso pode vir de variáveis de ambiente, cache do CMake, etc)

# == NÃO EDITAR AS LINHAS A SEGUIR para a extensão do VS Code do Raspberry Pi Pico funcionar ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

# Definir versões
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)

# Incluir o arquivo pico-vscode.cmake se existir
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# ====================================================================================
# Definir o tipo de board (raspberry pi pico W, por exemplo)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importar o SDK do Raspberry Pi Pico (deve vir antes de 'project')
include(pico_sdk_import.cmake)

# Definir o projeto
project(tarefa2 C CXX ASM)

# Inicializar o SDK do Raspberry Pi Pico
pico_sdk_init()

# Adicionar o executável (nome padrão é o nome do projeto)
add_executable(tarefa2 tarefa2.c)

# Definir o nome e versão do programa
pico_set_program_name(tarefa2 "tarefa2")
pico_set_program_version(tarefa2 "0.1")

# Configurar as saídas padrão (desabilitar UART, habilitar USB)
pico_enable_stdio_uart(tarefa2 0)
pico_enable_stdio_usb(tarefa2 1)

# Adicionar a biblioteca padrão para o projeto
target_link_libraries(tarefa2 pico_stdlib)

# Incluir diretórios de cabeçalhos (modifique conforme necessário)
target_include_directories(tarefa2 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  # Aqui você pode adicionar outros diretórios de cabeçalhos, por exemplo:
  # ${CMAKE_CURRENT_LIST_DIR}/include
)

# Se o projeto necessitar de outras bibliotecas externas, adicione-as abaixo:
 target_link_libraries(tarefa2
 hardware_pwm
 hardware_timer
 hardware_adc
 hardware_i2c
 hardware_spi
 hardware_pio
 hardware_clocks
 pico_stdlib)

# Adicionar saídas extras (ex: .uf2, .bin)
pico_add_extra_outputs(tarefa2)

# Se o projeto utilizar bibliotecas específicas, adicione-as aqui
# Exemplo de biblioteca adicional:
# target_link_libraries(tarefa2 pico_stdlib outras_bibliotecas)

