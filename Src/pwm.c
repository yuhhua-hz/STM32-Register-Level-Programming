/**
 * @file pwm.c
 * @brief Implementación de funciones para control PWM de LED
 * @details Contiene funciones para inicializar y controlar la salida PWM
 *          utilizando el timer TIM2 del microcontrolador STM32F070RB.
 *          Permite el control de brillo del LED conectado al pin PA5.
 */

#include "pwm.h"
#include "nucleo_conf.h"

/**
 * @brief Inicializa el periférico TIM2 para generar señal PWM en el LED
 * 
 * @details Esta función configura el timer TIM2 para generar una señal PWM
 *          de 100Hz en el pin PA5 (LED). La secuencia de configuración es:
 *          1. Habilita relojes para GPIOA y TIM2
 *          2. Configura el pin PA5 en modo función alternativa (AF2=TIM2_CH1)
 *          3. Configura el timer TIM2 con un preescalador y periodo
 *          4. Configura el canal 1 en modo PWM
 *          5. Habilita la salida y el contador
 *
 * @note La frecuencia PWM es de 100Hz con una resolución de 0-99 para el brillo
 * @return Ninguno
 */
void pwm_led_init(void)
{
    RCC_AHBENR |= (1 << 17);    // Activar reloj GPIOA
    RCC_APB1ENR |= (1 << 0);    // Activar reloj TIM2
    
    GPIOA_MODER &= ~(3 << 10);  // Limpiar bits PA5
    GPIOA_MODER |= (2 << 10);   // Settear modo funcion alternativa en PA5
    GPIOA_AFRL &= ~(0xF << 20); // Limpiar bit del 20 al 23
    GPIOA_AFRL |= (2 << 20);    // Activar AF2
    
    TIM2_PSC = 800 - 1;         // Preescalador: 8MHz/800 = 10kHz (frecuencia de conteo)
    TIM2_ARR = 100 - 1;         // Auto-reload: 10kHz/100 = 100Hz (frecuencia PWM)
                                // Determina 100 niveles posibles de brillo (0-99)

    TIM2_CCMR1 &= ~(7 << 4);    // Limpiar bits de modo de salida OC1M (bits 4-6)
    TIM2_CCMR1 |= (6 << 4);     // Configurar PWM modo 1 (110 binario) para OC1
    TIM2_CCMR1 |= (1 << 3);     // Habilitar precarga del registro OC1PE
    
    TIM2_CCER |= (1 << 0);       // Habilitar capture/compare
    
    TIM2_CCR1 = 0;              // Establecer ciclo de trabajo inicial a 0 (LED apagado)
    
    TIM2_CR1 |= (1 << 0);       // Habilitar contador del timer (CEN=1)
}

/**
 * @brief Establece el nivel de brillo del LED mediante PWM
 *
 * @details Controla el ciclo de trabajo del PWM para ajustar el brillo del LED.
 *          El valor se limita automáticamente al rango válido (0-99).
 *
 * @param brightness Nivel de brillo entre 0 (apagado) y 99 (máximo brillo)
 * 
 * @note Un valor de 0 apaga el LED, mientras que 99 representa el brillo máximo
 * @return Ninguno
 */
void set_led_brightness(uint8_t brightness)
{
    if (brightness > 99)
    {
        brightness = 99;    // Limitar el brillo al rango de 0-99
    }
    
    TIM2_CCR1 = brightness; // Establecer el ciclo de trabajo
}
