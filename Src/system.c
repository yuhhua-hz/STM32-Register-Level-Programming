/**
 * @file system.c
 * @brief Funciones de inicialización y temporización del sistema
 * @details Contiene funciones básicas del sistema para configuración del reloj,
 *          inicialización del SysTick, manejo de interrupciones y funciones de retardo
 */
#include "system.h"
#include "nucleo_conf.h"

/** @brief Contador global de milisegundos, incrementado por SysTick_Handler */
volatile uint32_t msTicks = 0;

/**
 * @brief Inicializa el temporizador SysTick
 * @details Configura el temporizador SysTick para generar una interrupción cada 1ms
 *          utilizando el reloj del sistema como fuente. El temporizador se configura
 *          para contar 8000 ciclos de reloj, lo que corresponde a 1ms a 8MHz.
 * @return Ninguno
 */
void systick_init(void)
{
    SYST_CSR |= SYST_CSR_CLKSOURCE; // Usar el reloj del sistema 8MHz
    SYST_RVR = 8000 - 1;            // Generar 1 interrupción cada 8000 ciclos
    SYST_CVR = 0;                   // Reiniciar contador
    SYST_CSR |= (SYST_CSR_ENABLE | SYST_CSR_TICKINT); // Activar contador y habilitar interrupción cuando llegue a cero
}

/**
 * @brief Manejador de interrupciones para SysTick
 * @details Esta función es llamada cuando el temporizador SysTick llega a cero.
 *          Incrementa el contador global msTicks.
 * @note Definida en el startup del microcontrolador
 * @return Ninguno
 */
void SysTick_Handler(void)
{
    ++msTicks;
}

/**
 * @brief Genera un retardo en milisegundos
 * @details Utiliza el temporizador SysTick para crear un retardo bloqueante
 *          durante el número de milisegundos especificado.
 * @param ms Número de milisegundos para el retardo
 * @return Ninguno
 */
void delay_ms(uint32_t ms)
{
    uint32_t start_ticks = msTicks;    // Captura el valor inicial del contador global
                                       // Esta referencia nos permite medir tiempo relativo
    
    while ((msTicks - start_ticks) < ms);  // Bucle de espera bloqueante
                                           // Comprueba continuamente la diferencia entre
                                           // el valor actual y el valor inicial
                                           // Sale cuando han pasado 'ms' milisegundos
}

/**
 * @brief Configura el reloj del sistema
 * @details Establece el reloj del sistema para funcionar a 8MHz utilizando
 *          el oscilador HSI interno. La función habilita el HSI, espera a que
 *          esté listo, y luego configura el sistema para usarlo como fuente
 *          de reloj.
 * @return Ninguno
 */
void clk_conf(void)
{
    
	RCC_CR |= RCC_CR_HSION;             // Configura el reloj del sistema a 8MHz

	while (!(RCC_CR & RCC_CR_HSIRDY));  // Esperar a que el oscilador sea estable

	RCC_CFGR &= ~RCC_CFGR_SW;           // Limpiar bits de seleccion de la fuente
	RCC_CFGR |= RCC_CFGR_SW_HSI;        // Seleccionar el HSI como reloj del sistema 8MHz

	while ((RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI); // Esperar confirmacion

	//RCC_CR &= ~(1UL << 24);           // Asegurar que PLL este desactivado
}
