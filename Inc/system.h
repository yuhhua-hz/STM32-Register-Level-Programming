#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>

void systick_init(void);
void delay_ms(uint32_t ms);
void clk_conf(void);

extern volatile uint32_t msTicks;

#endif // SYSTEM_H_
