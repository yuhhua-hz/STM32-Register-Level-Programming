/**
 * @file adc.c
 * @brief Implementación de funciones para el manejo del ADC
 * @details Contiene funciones para la configuración del ADC y lectura del sensor de temperatura
 *          interno del microcontrolador STM32F070RB.
 */

#include "adc.h"
#include "nucleo_conf.h"
#include "system.h"

/**
 * @brief Configura el ADC para la lectura del sensor de temperatura interno
 *
 * @details Esta función realiza las siguientes operaciones:
 *          1. Habilita el reloj del ADC
 *          2. Verifica y desactiva el ADC si está activo
 *          3. Realiza la calibración del ADC
 *          4. Activa el sensor de temperatura interno
 *          5. Configura el ADC en modo continuo
 *          6. Selecciona el canal apropiado y comienza la conversión
 *
 * @return Ninguno
 */
void adc_conf()
{
    RCC_APB2ENR |= (1 << 9);          // Habilitar el reloj del ADC
    
    if (ADC_CR & ADC_CR_ADEN)         // Comprobar si el bit ADEN del registro ADC_CR esta activo
    {                                 
        ADC_CR |= ADC_CR_ADDIS;       // Si esta activo enviar el bit ADDIS para desactivar el ADC
        while (ADC_CR & ADC_CR_ADEN); // Esperar mientras el ADC se desactive
    }
    
    ADC_CR |= ADC_CR_ADCAL;           // Empezar la calibracion
    while (ADC_CR & ADC_CR_ADCAL);    // Esperar a finalizar la calibracion
    
    ADC_CCR |= (1 << 23);             // Activar sensor de temperatura interno bit 23
    ADC_SMPR |= (7 << 0);             // Seleccion tiempo muestreo 239.5
    
    delay_ms(100);                    // Delay de estabilizacion
    
    ADC_CFGR1 |= (1 << 13);           // ADC en modo continuo
    

    ADC_CHSELR |= (1 << 16);          // Seleccionar canal de 16 bits
    

    ADC_CR |= ADC_CR_ADEN;            // Activar el ADC
    while (!(ADC_CR & ADC_CR_ADEN));  // Esperar hasta que se active
    
    delay_ms(20);                     // Delay de estabilizacion
    
    ADC_CR |= ADC_CR_ADSTART;         // Empezar conversion
}

/**
 * @brief Calcula la temperatura actual basándose en los valores del ADC
 *
 * @details Obtiene el valor actual del registro de datos del ADC y lo convierte
 *          a grados Celsius utilizando la siguiente fórmula:
 *          Temp = ((ADC_Value * VDD_APPLI / VDD_CALIB - TEMP30_CAL_VALUE) * 1000 / AVG_SLOPE) + 30
 *          
 *          Donde:
 *          - ADC_Value: Valor leído del registro de datos del ADC
 *          - VDD_APPLI: Voltaje de aplicación (mV)
 *          - VDD_CALIB: Voltaje de calibración (mV)
 *          - TEMP30_CAL_VALUE: Valor de calibración a 30°C (almacenado en memoria)
 *          - AVG_SLOPE: Pendiente promedio (μV/°C)
 *
 * @return int32_t Temperatura en grados Celsius
 */
int32_t get_temperature(void)
{
    uint32_t adc_value = ADC_DR; // Leer el valor del registro de datos
    return (((int32_t)adc_value * VDD_APPLI / VDD_CALIB - (int32_t)*TEMP30_CAL_ADDR) * 1000 / AVG_SLOPE) + 30;
}
