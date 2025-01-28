#include "pico/stdlib.h"

#define LED_PIN 13

// Configura a GPIO para saída
void setup_led() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

// Controla o LED para piscar
void blink_led(int duration_ms) {
    gpio_put(LED_PIN, 1);       // Acende o LED
    sleep_ms(duration_ms);      // Aguarda o tempo especificado
    gpio_put(LED_PIN, 0);       // Apaga o LED
}

// Transmite o sinal SOS em código Morse
void transmit_sos() {
    const int dot_time = 200;       // Ponto: 0,2 segundos
    const int dash_time = 800;      // Traço: 0,8 segundos
    const int gap_time = 125;       // Gap entre sinais
    const int letter_gap = 250;     // Gap entre letras
    const int sos_gap = 3000;       // Gap entre ciclos

    // Três pontos (S)
    for (int i = 0; i < 3; i++) {
        blink_led(dot_time);
        sleep_ms(gap_time);
    }
    sleep_ms(letter_gap);

    // Três traços (O)
    for (int i = 0; i < 3; i++) {
        blink_led(dash_time);
        sleep_ms(gap_time);
    }
    sleep_ms(letter_gap);

    // Três pontos (S)
    for (int i = 0; i < 3; i++) {
        blink_led(dot_time);
        sleep_ms(gap_time);
    }
    sleep_ms(sos_gap);
}

int main() {
    setup_led();
    while (true) {
        transmit_sos();
    }
    return 0;
}
