# STM32 Bare Metal Project 🚀

## Overview 📖

This project demonstrates bare metal programming on the STM32F070RB microcontroller, implementing a temperature monitoring and LED control system. The application showcases direct hardware manipulation through register-level programming, without relying on HAL or other abstraction layers.

The system allows users to interact via UART serial communication, where they can toggle temperature monitoring on/off and control the LED brightness level. This project serves as an educational platform to understand low-level peripheral configuration and bare metal development for STM32 microcontrollers.

## Project Goals 🎯

- Demonstrate bare metal programming techniques on STM32 microcontrollers
- Implement direct register access for peripheral configuration and control
- Create interactive user interfaces through UART communication
- Showcase real-time temperature monitoring using the internal temperature sensor
- Implement PWM-based LED brightness control with user input

## Features ✨

- **Temperature Monitoring**: Reads the internal temperature sensor and displays values in Celsius
- **LED Brightness Control**: Adjusts onboard LED brightness using PWM with 100 levels (0-99%)
- **Interactive UART Interface**: Accepts and processes user commands over serial communication
- **Precise Timing Control**: Implements millisecond-accurate delays using SysTick timer
- **Command System**:
  - `T` - Toggle temperature reading ON/OFF
  - `L<0-99>` - Set LED brightness level (0 = OFF, 99 = maximum brightness)

## Technologies Used 💻

### Hardware 🔧
- **Microcontroller**: STM32F070RB (ARM Cortex-M0 core)
- **Development Board**: NUCLEO-F070RB
- **Peripherals Used**:
  - Internal temperature sensor
  - Onboard LED (PA5)

### Software & Development Tools 🛠️
- **Development Environment**: STM32CubeIDE
- **Programming Language**: C
- **Build System**: Make

### Communication Protocols 📡
- **UART** (Universal Asynchronous Receiver-Transmitter):
  - Configuration: 9600 baud rate, 8 data bits, no parity, 1 stop bit (8N1)
  - Connected to USART2 (PA2 = TX, PA3 = RX)
- **PWM** (Pulse-Width Modulation):
  - 100Hz frequency (10kHz timer with prescaler of 100)
  - 100 brightness levels (0-99%)
  - Controlled through TIM2 channel 1 on PA5 (LED)
- **ADC** (Analog-to-Digital Converter):
  - Used to read internal temperature sensor
  - Continuous conversion mode

### Timing and System Management ⏱️
- **SysTick Timer**:
  - Configured to generate interrupts every 1ms (8000 cycles at 8MHz)
  - Used for precise non-blocking timing operations
  - Implements global millisecond counter (`msTicks`) for timing and delays
  - Provides `delay_ms()` function for synchronous blocking delays

## Project Structure 📂

- **Inc/**: Header files
  - `adc.h`: ADC configuration and temperature sensor interface
  - `nucleo_conf.h`: Peripheral register definitions and configurations
  - `pwm.h`: LED PWM control functions
  - `system.h`: System clock and timing functions
  - `uart.h`: UART communication interface
- **Src/**: Source files
  - `adc.c`: ADC and temperature sensor implementations
  - `main.c`: Main application logic and command processing
  - `pwm.c`: LED brightness control implementation
  - `syscalls.c` & `sysmem.c`: System calls for standard C library support
  - `system.c`: System timing and clock configuration
  - `uart.c`: Serial communication implementation
- **Startup/**: Microcontroller initialization code
  - `startup_stm32f070rbtx.s`: Assembly startup code

## Serial Output Example 📟

```
STM32F0xx Demo
T - to toggle temperature reading
L<0-99> - to set LED brightness

> T
Temperature reading ON
Temp: 25 degC
Temp: 25 degC
...

> T
Temperature reading OFF

> L50
LED command received, waiting for digits...50
LED brightness set to 50%
```

## License 📄

This project is provided as an educational resource.
