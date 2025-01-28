#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "ws2812.h"
#include "neopixel.h"
#include "buzzer.h"
#include "stdio.h"

// Definição de pinos
#define BUZZER_PIN 15  // Defina o pino GPIO para o buzzer

void beep_sound(uint pin, uint duration); // Renomeei a função para evitar conflito
void anima_flor(void);
void anima_ampulheta(void);
void anima_sorriso(void);

// Função para acionar o buzzer
void beep_sound(uint pin, uint duration) {
    gpio_put(pin, 1);  // Liga o buzzer
    sleep_ms(duration); // Duração do som
    gpio_put(pin, 0);  // Desliga o buzzer
}

// Função de animação de flor
void anima_flor() {
    int led_indices[] = {2, 7, 12, 17, 22, 27, 32, 37, 42, 47};
    int cores[][3] = {{0, 255, 0}, {255, 255, 0}, {255, 0, 0}};  // Verde, Amarelo, Vermelho
    int delay_time = 250;

    for (int i = 0; i < 10; i++) {
        int cor = i % 3;  // Alterna entre as cores
        npSetLED(led_indices[i], cores[cor][0], cores[cor][1], cores[cor][2]);
        npWrite();
        sleep_ms(delay_time);
    }

    // Ao final, toca o buzzer
    beep_sound(BUZZER_PIN, 500); // 500ms de som
}

// Função de animação de ampulheta
void anima_ampulheta() {
    int tempo_espera = 1000;  // Tempo entre os frames

    // Frame 1
    npClear();
    for (int i = 20; i < 25; i++) { // Linha 1 (topo) completa
        npSetLED(i, 255, 255, 0);
    }
    for (int i = 15; i < 20; i++) { // Linha 2 completa
        npSetLED(i, 255, 255, 0);
    }
    npWrite();
    sleep_ms(tempo_espera);

    // Frame 2
    npClear();
    for (int i = 20; i < 25; i++) { // Linha 1 completa
        npSetLED(i, 255, 255, 0);
    }
    for (int i = 15; i < 20; i++) { // Linha 2 completa
        npSetLED(i, 255, 255, 0);
    }
    npSetLED(12, 255, 255, 0); // Centro da linha 3
    npWrite();
    sleep_ms(tempo_espera);

    // Frame 3
    npClear();
    npSetLED(7, 255, 255, 0);
    npSetLED(12, 255, 255, 0);
    npSetLED(17, 255, 255, 0);
    npWrite();
    sleep_ms(tempo_espera);

    // Ao final da animação, toca o buzzer
    beep_sound(BUZZER_PIN, 500); // 500ms de som
}

// Função de animação de sorriso
void anima_sorriso() {
    int leds_sorriso[] = {6, 8, 11, 13, 14, 16, 19, 21};  // LEDs do sorriso
    int delay_time = 500;

    // Frame 1 - Começo do sorriso
    npClear();
    npSetLED(6, 255, 255, 0);  // Sorriso esquerdo
    npSetLED(8, 255, 255, 0);  // Sorriso direito
    npSetLED(11, 255, 255, 0); // Sorriso superior
    npSetLED(13, 255, 255, 0); // Sorriso inferior
    npWrite();
    sleep_ms(delay_time);

    // Frame 2 - Abertura do sorriso
    npClear();
    for (int i = 0; i < 6; i++) {
        npSetLED(leds_sorriso[i], 255, 255, 0);
    }
    npWrite();
    sleep_ms(delay_time);

    // Frame 3 - Sorriso completo
    npClear();
    for (int i = 0; i < 8; i++) {
        npSetLED(leds_sorriso[i], 255, 255, 0);
    }
    npWrite();
    sleep_ms(delay_time);

    // Finaliza com o som
    beep_sound(BUZZER_PIN, 500);
}

// Função principal de controle das teclas
int main() {
    // Inicializa os pinos e as variáveis
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    // Inicialização da matriz de LEDs
    npInit(5, 5); // Defina o tamanho da matriz 5x5

    while (true) {
        // Monitorando entradas do teclado
        char c = getchar();

        switch (c) {
            case '1':
                anima_flor();
                break;

            case '2':
                anima_ampulheta();
                break;

            case '3':
                anima_sorriso();
                break;

            case '5':
                // Ativa o buzzer manualmente
                beep_sound(BUZZER_PIN, 500); // 500ms de som
                break;

            case 'd':
                // Desliga todos os LEDs
                npClear();
                npWrite();
                break;

            default:
                break;
        }
    }
}
