#include "ssd1306.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <string.h>

// Função para inicializar o display
SSD1306 ssd1306_create(i2c_inst_t *i2c, uint8_t address, uint8_t width, uint8_t height) {
    SSD1306 display = {i2c, address, width, height};
    // Inicialização do display, aqui você pode colocar o código para configurar o SSD1306 via I2C
    uint8_t init_sequence[] = {
        0xAE,   // Display OFF
        0xD5, 0x80,   // Set display clock divide
        0xA8, 0x3F,   // Set multiplex ratio
        0xD3, 0x00,   // Set display offset
        0x40,         // Set start line
        0x8D, 0x14,   // Charge pump
        0x20, 0x00,   // Memory addressing mode (horizontal addressing)
        0xA1,         // Set segment re-map
        0xC8,         // Set COM output scan direction
        0xDA, 0x12,   // Set COM pins hardware configuration
        0x81, 0x7F,   // Set contrast control
        0xD9, 0xF1,   // Set pre-charge period
        0xDB, 0x40,   // Set VCOMH deselect level
        0xA4,         // Entire display ON
        0xA6,         // Set normal display (not inverted)
        0xAF          // Display ON
    };
    i2c_write_blocking(i2c, address, init_sequence, sizeof(init_sequence), false);
    return display;
}

// Função para limpar o display
void ssd1306_clear(SSD1306 *display) {
    uint8_t clear_data[SSD1306_WIDTH * SSD1306_HEIGHT / 8] = {0};  // Cria um buffer de zeros para limpar o display
    for (uint8_t page = 0; page < 8; page++) {
        uint8_t cmd[] = {0xB0 + page, 0x00, 0x10};  // Definir endereço da página e colunas
        i2c_write_blocking(display->i2c, display->address, cmd, sizeof(cmd), false);
        i2c_write_blocking(display->i2c, display->address, clear_data + page * SSD1306_WIDTH, SSD1306_WIDTH, false);
    }
}

// Função para desenhar uma string no display
void ssd1306_draw_string(SSD1306 *display, uint8_t x, uint8_t y, uint8_t size, const char *str) {
    // A função para desenhar a string será implementada aqui
    // Para simplificar, podemos enviar os dados como texto com o tamanho selecionado
    // Implementar o código para enviar caracteres ao display com I2C
}

// Função para atualizar o display
void ssd1306_show(SSD1306 *display) {
    // Atualiza o display com o conteúdo que foi desenhado
    // Em um display SSD1306, isso geralmente é feito com um comando de atualização de memória
    uint8_t cmd[] = {0xAF};  // Display ON
    i2c_write_blocking(display->i2c, display->address, cmd, sizeof(cmd), false);
}
