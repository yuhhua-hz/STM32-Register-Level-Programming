#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

void pwm_led_init(void);
void set_led_brightness(uint8_t brightness);

#endif // PWM_H_
