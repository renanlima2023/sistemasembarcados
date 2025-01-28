#ifndef SSD1306_H
#define SSD1306_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

// Definição do endereço I2C do display (pode variar dependendo do modelo)
#define SSD1306_ADDR 0x3C

// Resolução do display (exemplo 128x64)
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

// Estrutura que representará o display
typedef struct {
    i2c_inst_t *i2c;    // Instância do I2C (pico0 ou pico1)
    uint8_t address;    // Endereço I2C do display
    uint8_t width;      // Largura do display
    uint8_t height;     // Altura do display
} SSD1306;

// Função para inicializar o display
SSD1306 ssd1306_create(i2c_inst_t *i2c, uint8_t address, uint8_t width, uint8_t height);

// Função para limpar o display
void ssd1306_clear(SSD1306 *display);

// Função para mostrar uma string no display
void ssd1306_draw_string(SSD1306 *display, uint8_t x, uint8_t y, uint8_t size, const char *str);

// Função para atualizar o display
void ssd1306_show(SSD1306 *display);

#endif // SSD1306_H
