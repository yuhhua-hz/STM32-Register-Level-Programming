/**
 * @file main.c
 * @brief Aplicación principal para STM32F0xx que demuestra la lectura de temperatura y control de LED
 * @details Esta aplicación implementa un sistema de monitoreo de temperatura y control
 *          de brillo de LED mediante comunicación UART. Soporta comandos interactivos
 *          para activar/desactivar la lectura de temperatura y ajustar el brillo del LED.
 */

#include <stdint.h>
#include <stdio.h>
#include "nucleo_conf.h"
#include "system.h"
#include "adc.h"
#include "uart.h"
#include "pwm.h"

/**
 * @brief Función principal de la aplicación
 * 
 * @details Inicializa los periféricos necesarios (reloj, systick, ADC, UART, PWM),
 *          configura la interfaz de usuario y maneja los comandos recibidos a través
 *          de la comunicación UART. Implementa dos funcionalidades principales:
 *          1. Monitoreo de temperatura (activado/desactivado con comando 'T')
 *          2. Control del brillo del LED (ajustado con comando 'L' seguido de un número de 0-99)
 *
 * @return int Nunca retorna (bucle infinito)
 */
int main(void)
{
    // Inicialización de periféricos
    clk_conf();
    systick_init();
    adc_conf();
    pwm_led_init();
    uart_conf();

    // Limpiar posibles datos residuales en el buffer
    while (uart_data_available()) {
        uart_receive_char();
    }
    delay_ms(50); // Delay de estabilizacion
    
    char buffer[32];                 // Buffer para formatear mensajes de salida
    char received_char;              // Carácter recibido por UART desde el puerto serie
    int32_t temperature;             // Valor de temperatura medido
    uint8_t temp_reading_active = 0; // Estado del monitoreo de temperatura (0=OFF, 1=ON)

    // Mostrar de mensajes de inicio y menú de opciones
    uart_send_string("STM32F0xx Demo\r\n");
    uart_send_string("T - to toggle temperature reading\r\n");
    uart_send_string("L<0-99> - to set LED brightness\r\n");

    // Bucle principal de la aplicación
    while (1)
    {
        // Procesamiento de comandos cuando hay datos disponibles
        if (uart_data_available())
        {
            received_char = uart_receive_char();

            // Comando T: Alternar estado de lectura de temperatura
            if (received_char == 'T' || received_char == 't')
            {
                temp_reading_active = !temp_reading_active; // Conmutar el estado anterior ON-> OFF -> ON

                if (temp_reading_active) // Motrar el mensaje segun el estado del flag
                {
                    uart_send_string("Temperature reading ON\r\n");
                }
                else
                {
                    uart_send_string("Temperature reading OFF\r\n");
                }
            }
            // Comando L: Configurar brillo del LED con PWM
            else if (received_char == 'L' || received_char == 'l')
            {
                // Mostrar mensaje de confirmación
                uart_send_string("LED command received, waiting for digits...\r\n");
                
                char brightness_str[3] = {0}; // Almacena los dígitos recibidos (2 dígitos + null)
                int idx = 0;                  // Índice de control para el array de brillo
                                
                // Esperar dígitos con un timeout no bloqueante
                uint32_t start_time = msTicks;
                while (idx < 2 && (msTicks - start_time) < 5000) // 5 segundos de timeout
                {
                    if (uart_data_available())
                    {
                        char c = uart_receive_char();
                        
                        USART_TDR = c; // Imprimir el caracter recibido por puerto serie
                        while (!(USART_ISR & (1 << 7))); // Esperar hasta que se complete la transmisión
                        
                        // Si es un dígito, almacenarlo
                        if (c >= '0' && c <= '9')
                        {
                            brightness_str[idx++] = c;
                            delay_ms(5);
                        }
                        else
                        {
                            break; // Salir si no es un dígito
                        }
                    }
                }
                
                // Procesar los dígitos recibidos si hay alguno
                if (idx > 0)
                {
                    uint8_t brightness = 0;
                    // Convertir string a valor numérico
                    for (uint8_t i = 0; i < idx; ++i)
                    {
                        brightness = brightness * 10 + (brightness_str[i] - '0');
                    }
                    
                    // Aplicar el brillo al LED
                    set_led_brightness(brightness);
                    
                    // Informar del nuevo valor de brillo
                    sprintf(buffer, "\r\nLED brightness set to %d%%\r\n", brightness);
                    uart_send_string(buffer);
                }
                else
                {
                    uart_send_string("No digits received after L command\r\n"); // No hay digitos
                }
            }
        }

        // Si la lectura de temperatura está activa, mostrar valor cada segundo
        if (temp_reading_active)
        {
            temperature = get_temperature();
            
            sprintf(buffer, "Temp: %ld degC\r\n", temperature);
            uart_send_string(buffer);
            
            delay_ms(1000);
        }
    }
}
