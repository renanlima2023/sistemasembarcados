#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "count.h"

#define IS_RGBW false
#define WS2812_PIN 7

#define RED_LED_PIN 13
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

static int count = 0;

bool led_buffer[NUM_PIXELS]; // Buffer de LEDs
bool button_pressed = false; // Flag para indicar que um botão foi pressionado

void gpio_irq_handler(uint gpio, uint32_t events); // Função de tratamento de interrupção
void copy_array(bool *dest, const bool *src); // Função para copiar um array para outro

static inline void put_pixel(uint32_t pixel_grb); // Função para enviar um pixel para o buffer
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b); // Função para converter um pixel para um inteiro
void set_one_led(uint8_t r, uint8_t g, uint8_t b); // Função para definir a cor de todos os LEDs
uint32_t rainbow_color(int index); // Função para gerar as cores do arco-íris

int main()
{
    // Inicialização do PIO
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    stdio_init_all();

    // Configuração do pino do LED vermelho
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);

    // Configuração dos pinos dos botões
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Inicialização do programa do WS2812
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicialização do buffer de LEDs
    copy_array(led_buffer, zero);
    set_one_led(0, 0, 255); // Cor Azul por padrão, para mostrar que está em espera

    while (true) {
        if (button_pressed) { // Enquanto o botão estiver pressionado, mostra azul
            set_one_led(0, 0, 255); // Cor Azul
        } else {
            // Quando o botão for solto, mostra o número com arco-íris
            for (int i = 0; i < NUM_PIXELS; i++) {
                if (led_buffer[i]) {
                    uint32_t rainbowColor = rainbow_color(i + count);  // Gera a cor do arco-íris com base no índice
                    put_pixel(rainbowColor);  // Aplica a cor no LED ativo
                } else {
                    put_pixel(0);  // Desliga o LED
                }
            }
        }

        // Pisca o LED vermelho 5 vezes a cada 5 segundos
        for (int i = 0; i < 5; i++) {
            gpio_put(RED_LED_PIN, true);
            sleep_ms(50);
            gpio_put(RED_LED_PIN, false);
            sleep_ms(150);
        }
    }
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    static uint32_t last_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    // Debouncing: Ignora pressões muito rápidas
    if (current_time - last_time < 400) {
        return;
    }

    last_time = current_time;

    // Verifica se o botão foi pressionado ou liberado
    if (gpio == BUTTON_A_PIN || gpio == BUTTON_B_PIN) {
        button_pressed = !button_pressed;  // Alterna o estado do botão
    }

    // Atualiza o contador dependendo do botão pressionado
    if (gpio == BUTTON_A_PIN) {
        count++;
        if (count > 9) count = 0; // Reseta o contador após 9
    } else if (gpio == BUTTON_B_PIN) {
        count--;
        if (count < 0) count = 9; // Reseta o contador para 9 quando menor que 0
    }

    // Define o estado do buffer de LEDs baseado no contador
    switch (count) {
        case 0: copy_array(led_buffer, zero); break;
        case 1: copy_array(led_buffer, um); break;
        case 2: copy_array(led_buffer, dois); break;
        case 3: copy_array(led_buffer, tres); break;
        case 4: copy_array(led_buffer, quatro); break;
        case 5: copy_array(led_buffer, cinco); break;
        case 6: copy_array(led_buffer, seis); break;
        case 7: copy_array(led_buffer, sete); break;
        case 8: copy_array(led_buffer, oito); break;
        case 9: copy_array(led_buffer, nove); break;
    }
}

// Função para calcular a cor do arco-íris com base no índice
uint32_t rainbow_color(int index) {
    uint8_t r = 0, g = 0, b = 0;

    switch (index % 7) {
        case 0: r = 255; g = 0; b = 0; break;    // Vermelho
        case 1: r = 255; g = 127; b = 0; break;  // Laranja
        case 2: r = 255; g = 255; b = 0; break;  // Amarelo
        case 3: r = 0; g = 255; b = 0; break;    // Verde
        case 4: r = 0; g = 0; b = 255; break;    // Azul
        case 5: r = 75; g = 0; b = 130; break;   // Índigo
        case 6: r = 148; g = 0; b = 211; break;  // Violeta
    }

    return urgb_u32(r, g, b);  // Retorna a cor como uint32
}

void copy_array(bool *dest, const bool *src)
{
    for (int i = 0; i < NUM_PIXELS; i++) {
        dest[i] = src[i];
    }
}

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); // Envia o pixel para o buffer
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b); // Converte os componentes RGB para um inteiro
}

void set_one_led(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = urgb_u32(r, g, b);

    // Envia o valor de cor para todos os LEDs conforme o buffer
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (led_buffer[i]) {
            put_pixel(color); // Acende o LED se estiver no buffer
        } else {
            put_pixel(0); // Desliga o LED se não estiver no buffer
        }
    }
}
