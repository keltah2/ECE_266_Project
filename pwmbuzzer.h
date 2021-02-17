/*
 * pwmbuzzer.h
 *
 */

#ifndef PWMBUZZER_H_
#define PWMBUZZER_H_

typedef struct {
    uint32_t pwmPeriod; // frequency
    uint32_t pwmDutyCycle;  // volume
} pwm_buzz;

 /** Initialize the timer PWM functions connected to the buzzer.
*/
void pwmBuzzInit();

/*
 * Set the intensity of the buzzer
 */
void pwmBuzzSetIntensity(uint32_t freq, uint32_t dutyCycle);

#endif /* PWMBUZZER_H_ */
