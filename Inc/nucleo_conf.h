/**
 * @file nucleo_conf.h
 * @brief Definiciones de macros para acceso a periféricos del microcontrolador STM32F070RB
 * @details Contiene las definiciones de direcciones de memoria, registros y valores de bits
 *          para el acceso directo a los periféricos del microcontrolador STM32F070RB.
 *          Incluye definiciones para SysTick, RCC, ADC, GPIO, USART y Timer.
 *          Las direcciones y bits están basados en la documentación de referencia:
 *          "STM32F070x6/xB Reference Manual (RM0360)", "User Manual (UM1724)" y
 *          "STM32F0 series Cortex-M0 programming manual (PM0215)"
 */

#ifndef NUCLEO_CONF_H_
#define NUCLEO_CONF_H_

#include <stdint.h>

// Registros del SysTick - Temporizador interno de 24 bits
#define SYST_CSR 			(*(volatile uint32_t *)0xE000E010) // Control and Status Register - Controla la habilitación, interrupciones y fuente de reloj
#define SYST_RVR			(*(volatile uint32_t *)0xE000E014) // Reload Value Register - Valor de recarga para el contador (define el periodo)
#define SYST_CVR			(*(volatile uint32_t *)0xE000E018) // Current Value Register - Valor actual del contador (lectura/escritura lo resetea a 0)

// Registros del RCC - Controla todos los relojes del sistema
#define RCC_BASE 			0x40021000 // Dirección base del periférico RCC										
#define RCC_CR				*(volatile uint32_t*) (RCC_BASE + 0x00) // Clock Control Register - Configura y habilita los osciladores
#define RCC_CFGR			*(volatile uint32_t*) (RCC_BASE + 0x04) // Clock Configuration Register - Configura los divisores y multiplexores de reloj
#define RCC_AHBENR			*(volatile uint32_t*) (RCC_BASE + 0x14) // AHB Peripheral Clock Enable Register - Habilita relojes de periféricos en bus AHB
#define RCC_APB2ENR			*(volatile uint32_t*) (RCC_BASE + 0x18) // APB2 Peripheral Clock Enable Register - Habilita relojes de periféricos en bus APB2
#define RCC_APB1ENR			*(volatile uint32_t*) (RCC_BASE + 0x1C) // APB1 Peripheral Clock Enable Register - Habilita relojes de periféricos en bus APB1

// Registros ADC - Conversión de señales analógicas a digitales
#define ADC_BASE			0x40012400 // Dirección base del periférico ADC
#define ADC_CHSELR			*(volatile uint32_t*) (ADC_BASE + 0x28) // Channel Selection Register - Selecciona los canales para conversión
#define ADC_SMPR			*(volatile uint32_t*) (ADC_BASE + 0x14) // Sampling Time Register - Configura tiempo de muestreo para los canales
#define ADC_CCR				*(volatile uint32_t*) (ADC_BASE + 0x308) // Common Configuration Register - Configuración común para todos los ADCs
#define ADC_CR				*(volatile uint32_t*) (ADC_BASE + 0x08) // Control Register - Control principal del ADC (encendido, inicio conversión)
#define ADC_CFGR1			*(volatile uint32_t*) (ADC_BASE + 0x0C) // Configuration Register 1 - Configura modo de conversión y resolución
#define ADC_DR				*(volatile uint32_t*) (ADC_BASE + 0x40) // Data Register - Contiene el resultado de la última conversión

// Registros GPIOA - Control de pines de entrada/salida del puerto A (LED, USART2)
#define GPIOA_BASE			0x48000000 // Dirección base del periférico GPIOA
#define GPIOA_MODER			*(volatile uint32_t*) (GPIOA_BASE + 0x00) // Mode Register - Configura modo de operación (entrada, salida, función alterna, analógico)
#define GPIOA_AFRL			*(volatile uint32_t*) (GPIOA_BASE + 0x20) // Alternate Function Low Register - Selecciona función alterna para pines 0-7

// Registros USART2 (RX PA2, TX PA3) - Comunicación serie asíncrona
#define USART2_BASE			0x40004400 // Dirección base del periférico USART2
#define USART_CR1			*(volatile uint32_t*) (USART2_BASE + 0x00) // Control Register 1 - Habilita USART, configura bits de datos, paridad
#define USART_CR2 			*(volatile uint32_t*) (USART2_BASE + 0x04) // Control Register 2 - Configura bits de parada y otras opciones
#define USART_BRR			*(volatile uint32_t*) (USART2_BASE + 0x0C) // Baud Rate Register - Define la velocidad de comunicación
#define USART_ISR			*(volatile uint32_t*) (USART2_BASE + 0x1C) // Interrupt and Status Register - Indica estado (TX completo, RX disponible)
#define USART_RDR           *(volatile uint32_t*) (USART2_BASE + 0x24) // Receive Data Register - Contiene el byte recibido
#define USART_TDR			*(volatile uint32_t*) (USART2_BASE + 0x28) // Transmit Data Register - Registro para enviar datos

// Registros TIM2 (Timer 2) - Temporizador de propósito general
#define TIM2_BASE           0x40000000 // Dirección base del periférico Timer 2
#define TIM2_CR1            *(volatile uint32_t*) (TIM2_BASE + 0x00) // Control Register 1 - Configuración básica del timer (habilitación, modo, etc.)
#define TIM2_CCMR1          *(volatile uint32_t*) (TIM2_BASE + 0x18) // Capture/Compare Mode Register 1 - Configura el modo de canales 1 y 2
#define TIM2_CCER           *(volatile uint32_t*) (TIM2_BASE + 0x20) // Capture/Compare Enable Register - Habilita salidas y polaridad
#define TIM2_PSC            *(volatile uint32_t*) (TIM2_BASE + 0x28) // Prescaler Register - Divide el reloj de entrada al timer
#define TIM2_ARR            *(volatile uint32_t*) (TIM2_BASE + 0x2C) // Auto-Reload Register - Determina el periodo del timer
#define TIM2_CCR1           *(volatile uint32_t*) (TIM2_BASE + 0x34) // Capture/Compare Register 1 - Contiene valor de comparación para el canal 1

// Bits de control para registro SysTick CSR
#define SYST_CSR_ENABLE     (0x1U << 0)     // Bit 0: Habilita el contador SysTick (1=activado, 0=desactivado)
#define SYST_CSR_TICKINT    (0x1U << 1)     // Bit 1: Habilita la interrupción SysTick cuando el contador llega a 0
#define SYST_CSR_CLKSOURCE  (0x1U << 2)     // Bit 2: Selecciona la fuente de reloj (1=reloj del procesador, 0=reloj externo)

// Bits de control para registros RCC
#define RCC_CR_HSION		(0x1U << 0)     // Bit 0: Habilita el oscilador interno de alta velocidad (HSI)
#define RCC_CR_HSIRDY		(0x1U << 1)     // Bit 1: Flag que indica si el HSI está estable y listo (1=listo)
#define RCC_CFGR_SW			(0x3U << 0)     // Bits 0-1: Mascara de selección de la fuente de reloj del sistema
#define RCC_CFGR_SW_HSI		(0x0U << 0)     // 00: Selecciona HSI como fuente de reloj del sistema
#define RCC_CFGR_SWS        (0x3U << 2)    // Bits 2-3: Máscara de estado de la fuente de reloj del sistema
#define RCC_CFGR_SWS_HSI    (0x0U << 2)    // 00: HSI está siendo usado como reloj del sistema

// Bits de control para registros ADC
#define ADC_CR_ADEN			(0x1U << 0)     // Bit 0: Habilita el ADC (1=activado)
#define ADC_CR_ADDIS		(0x1U << 1)     // Bit 1: Deshabilita el ADC (1=deshabilitar)
#define ADC_CR_ADSTART		(0x1U << 2)     // Bit 2: Inicia la conversión ADC (1=iniciar)
#define ADC_CR_ADCAL		(0x1U << 31)    // Bit 31: Inicia la calibración del ADC (1=calibrar)

// Calibracion sensor de temperatura interno - Para conversión de valores ADC a temperatura en grados Celsius
#define TEMP30_CAL_ADDR     ((uint16_t*) ((uint32_t) 0x1FFFF7B8))       // Dirección de memoria para valor de calibración a 30°C (programado en fábrica)
#define VDD_CALIB           ((uint32_t) (3300))                         // Voltaje de calibración en mV utilizado por el fabricante
#define VDD_APPLI           ((uint32_t) (3300))                         // Voltaje real de funcionamiento en mV
#define AVG_SLOPE           ((uint32_t) (5336))                         // Pendiente promedio en μV/°C para sensor de temperatura

#endif // NUCLEO_CONF_H_
