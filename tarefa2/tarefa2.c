#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include <string.h>

#define LED_GREEN_PIN 11
#define LED_BLUE_PIN 12
#define LED_RED_PIN 13

#define BUZZER_PIN 21
#define BUZZER_FREQ 1000  // Frequência desejada para o buzzer (1kHz)

const char led_blue[] = "BLUE";
const char led_red[] = "RED";
const char led_green[] = "GREEN";
const char buzzer[] = "BUZZER";

char command[10];

void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();
    uint clock_div = clock_get_hz(clk_sys) / BUZZER_FREQ / 4096;  // Cálculo do divisor de clock
    pwm_config_set_clkdiv(&config, clock_div);
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);  // Inicializa o buzzer com duty cycle de 0 (desligado)
}

void beep(uint pin, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);

    pwm_set_gpio_level(pin, 2048);  // 50% de duty cycle (valor de 2048 para 4095)
    sleep_ms(duration_ms);  // Duração do beep
    pwm_set_gpio_level(pin, 0);  // Desliga o buzzer
}

void to_uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
}

int main() {
    stdio_init_all();  // Inicializa o serial

    pwm_init_buzzer(BUZZER_PIN);

    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);

    while (true) {
        printf("Insira um comando: ");
        int i = 0;
        while (true) {
            int c = getchar_timeout_us(100000);
            if (c != PICO_ERROR_TIMEOUT) {
                if (c == '\n' || c == '\r') {
                    command[i] = '\0';
                    break;
                }
                command[i++] = (char)c;
                if (i >= sizeof(command) - 1) {
                    command[i] = '\0';
                    break;
                }
            }
        }

        to_uppercase(command);

        if (strcmp(command, led_red) == 0) {
            printf("Ligando LED vermelho\n");
            gpio_put(LED_RED_PIN, 1);
            sleep_ms(1000);
            gpio_put(LED_RED_PIN, 0);
        } else if (strcmp(command, led_blue) == 0) {
            printf("Ligando LED azul\n");
            gpio_put(LED_BLUE_PIN, 1);
            sleep_ms(1000);
            gpio_put(LED_BLUE_PIN, 0);
        } else if (strcmp(command, led_green) == 0) {
            printf("Ligando LED verde\n");
            gpio_put(LED_GREEN_PIN, 1);
            sleep_ms(1000);
            gpio_put(LED_GREEN_PIN, 0);
        } else if (strcmp(command, buzzer) == 0) {
            printf("Ativando buzzer\n");
            beep(BUZZER_PIN, 500);  // Ativa o buzzer por 500ms
        } else {
            printf("Comando desconhecido: %s\n", command);
        }

        sleep_ms(1000);  // Espera 1 segundo antes de ler um novo comando
    }
}
