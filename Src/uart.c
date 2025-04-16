/**
 * @file uart.c
 * @brief Implementación de funciones para la comunicación serie UART
 * @details Contiene las funciones para configurar y utilizar la comunicación serie UART
 *          a través del periférico USART2 del microcontrolador STM32F070RB.
 *          La comunicación está configurada para 9600 baudios, 8 bits de datos, 
 *          sin paridad y 1 bit de parada (8N1).
 */

#include "uart.h"
#include "nucleo_conf.h"

/**
 * @brief Configura el periférico UART2 para comunicación serie
 *
 * @details Esta función realiza la siguiente secuencia de configuración:
 *          1. Habilita el reloj para el puerto GPIOA (pines PA2 = TX, PA3 = RX)
 *          2. Configura los pines PA2 y PA3 en modo función alterna (AF1)
 *          3. Habilita el reloj para el periférico USART2
 *          4. Configura USART2 para comunicación 8N1 a 9600 baudios
 *          5. Habilita transmisión y recepción
 *          6. Activa el periférico USART2
 *
 * @note La velocidad de comunicación está configurada para 9600 baudios con
 *       un reloj del sistema de 8MHz
 * 
 * @return Ninguno
 */
void uart_conf()
{
    RCC_AHBENR |= (1 << 17);    // Activar reloj GPIOA
    
    GPIOA_MODER &= ~(3 << 4);   // Limpiar PA2 (TX)
    GPIOA_MODER |= (2 << 4);    // Settear modo funcion alternativa en PA2
    GPIOA_AFRL &= ~(0xF << 8);  // Limpiar bits del 8 al 11
    GPIOA_AFRL |= (1 << 8);     // Activar AF1 para PA2
    
    GPIOA_MODER &= ~(3 << 6);   // Limpiar PA3 (RX)
    GPIOA_MODER |= (2 << 6);    // Settear modo funcion alternativa en PA3
    GPIOA_AFRL &= ~(0xF << 12); // Limpiar bits del 12 a 15
    GPIOA_AFRL |= (1 << 12);    // Activar AF1 para PA3
    

    RCC_APB1ENR |= (1 << 17);   // Habilitar reloj UART2
    
    USART_CR1 &= ~(1 << 0);     // Desactivar UART
    
    USART_CR1 &= ~(1 << 15);    // Sobremuestreo a 16 bits
    
    // Longitud de palabra: 8 bits (M[1:0] = 00) - bits 12 y 28 en CR1
    USART_CR1 &= ~(1 << 12);    // M0 = 0
    USART_CR1 &= ~(1 << 28);    // M1 = 0
    
    USART_CR1 &= ~(1 << 10);    // Bit de paridad desactivado
    
    // Bits de parada: 1 bit de parada (STOP = 00) - bits 12-13 en CR2
    USART_CR2 &= ~(3 << 12);    // STOP[1:0] = 00
    
    USART_BRR = 0x341;          // BRR 9600 @ 8Mhz
                                // Resumen del formato:
                                // 8 Bits de datos - Sin paridad - 1 bit de parada -
                                // 9600 baudios - 16 bits de sobremuestreo

    USART_CR1 |= (1 << 3);      // Activar transmision
    
    USART_CR1 |= (1 << 2);      // Activar recepcion

    USART_CR1 |= (1 << 0);      // Activar UART2  
}

/**
 * @brief Envía una cadena de caracteres por UART
 *
 * @details Transmite cada carácter de la cadena de forma secuencial
 *          y espera a que cada transmisión se complete antes de
 *          enviar el siguiente carácter.
 *
 * @param str Puntero a la cadena de caracteres a enviar (terminada en NULL)
 *
 * @note Esta función es bloqueante, no retorna hasta que todos los
 *       caracteres hayan sido transmitidos.
 *
 * @return Ninguno
 */
void uart_send_string(const char *str)
{
    // Recorre cada carácter de la cadena hasta encontrar el terminador nulo
    for (int i = 0; str[i]; i++)
    {
        // Envía el carácter actual al registro de transmisión
        USART_TDR = str[i];
        
        // Espera hasta que el bit TXE indique que el registro está vacío y listo para enviar
        while (!(USART_ISR & (1 << 7)));
    }
}

/**
 * @brief Recibe un carácter por UART si hay datos disponibles
 *
 * @details Comprueba si hay datos disponibles en el buffer de recepción
 *          y devuelve el carácter recibido si existe.
 *
 * @note Esta función no es bloqueante. Si no hay datos disponibles,
 *       devuelve 0 inmediatamente.
 *
 * @return El carácter recibido o 0 si no hay datos disponibles
 */
char uart_receive_char(void)
{
    // Verifica si hay datos disponibles (bit RXNE activado)
    if (USART_ISR & (1 << 5))
    {
        // Lee y retorna el carácter recibido
        return (char)USART_RDR;
    }
    
    // Si no hay datos, retorna 0
    return 0;
}

/**
 * @brief Comprueba si hay datos disponibles para leer en el UART
 *
 * @details Verifica el bit RXNE (Read data register not empty)
 *          en el registro de estado USART_ISR.
 *
 * @return 1 si hay datos disponibles para leer, 0 en caso contrario
 */
uint8_t uart_data_available(void)
{
    // Retorna 1 si hay datos disponibles (bit RXNE activado), 0 en caso contrario
    return (USART_ISR & (1 << 5)) ? 1 : 0;
}
