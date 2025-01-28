#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico-ssd1306/ssd1306.h"

#define SDA_PIN 4  // Pinos SDA e SCL do display
#define SCL_PIN 5
#define I2C_PORT i2c0

#define led_pin_red 13
#define led_pin_green 11
#define led_pin_blue 12

// Função para gravar dados no arquivo CSV
void salvar_dados_csv(FILE *file, int temperatura, int umidade, int luminosidade) {
    fprintf(file, "%d,%d,%d\n", temperatura, umidade, luminosidade);
    fflush(file); // Garante que os dados sejam salvos imediatamente
}

int main() {
    stdio_init_all();

    // Inicializa os LEDs
    gpio_init(led_pin_red);
    gpio_init(led_pin_green);
    gpio_init(led_pin_blue);
    gpio_set_dir(led_pin_red, GPIO_OUT);
    gpio_set_dir(led_pin_green, GPIO_OUT);
    gpio_set_dir(led_pin_blue, GPIO_OUT);

    // Inicializa o I2C para o display
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    SSD1306 display = ssd1306_create(I2C_PORT, 0x3C, 128, 64); // Display OLED
    ssd1306_clear(&display);

    // Abre o arquivo CSV para salvar os dados
    FILE *file = fopen("/sdcard/dados_sensores.csv", "w");
    if (!file) {
        printf("Erro ao abrir o arquivo CSV.\n");
        return 1;
    }

    // Escreve o cabeçalho no arquivo CSV
    fprintf(file, "Temperatura,Umidade,Luminosidade\n");

    while (true) {
        // Gera dados simulados
        int temperatura = rand() % 21 + 20; // Temperatura entre 20 e 40 °C
        int umidade = rand() % 61 + 30;     // Umidade entre 30 e 90%
        int luminosidade = rand() % 101;    // Luminosidade entre 0 e 100%

        // Controle dos LEDs
        gpio_put(led_pin_red, temperatura > 30);
        gpio_put(led_pin_green, umidade > 50);
        gpio_put(led_pin_blue, luminosidade > 50);

        // Exibe os dados no console
        printf("Temperatura: %d °C\n", temperatura);
        printf("Umidade: %d %%\n", umidade);
        printf("Luminosidade: %d %%\n", luminosidade);

        // Salva os dados no arquivo CSV
        salvar_dados_csv(file, temperatura, umidade, luminosidade);

        // Exibe os dados no display
        char buffer[64];
        ssd1306_clear(&display);
        snprintf(buffer, sizeof(buffer), "Temp: %d C", temperatura);
        ssd1306_draw_string(&display, 0, 0, 1, buffer);
        snprintf(buffer, sizeof(buffer), "Umid: %d %%", umidade);
        ssd1306_draw_string(&display, 0, 10, 1, buffer);
        snprintf(buffer, sizeof(buffer), "Lum: %d %%", luminosidade);
        ssd1306_draw_string(&display, 0, 20, 1, buffer);
        ssd1306_show(&display);

        // Aguarda 1 segundo
        sleep_ms(1000);
    }

    fclose(file); // Fecha o arquivo CSV
    return 0;
}
