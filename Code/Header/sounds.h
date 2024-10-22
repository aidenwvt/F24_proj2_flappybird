/*
 * Sounds.h
 *
 *  Created on: Oct 22, 2024
 *      Author: Aiden Wiehn
 */

#ifndef CODE_SOUNDS_H_
#define CODE_SOUNDS_H_

#include <HAL/HAL.h>
#include <Code/Header/proj2_app.h>

#define PWM_PERIOD_CYCLES 284

// Duty cycle as a fraction (between 0 and 1)
#define DUTY_CYCLE_FRACTION 0.5

// The number of on cycles (Timer_A counter cycles)
#define BLUE_ON_CYCLES (DUTY_CYCLE_FRACTION * PWM_PERIOD_CYCLES)

// The number of off cycles (Timer_A counter cycles)
#define BLUE_OFF_CYCLES ((1 - DUTY_CYCLE_FRACTION) * PWM_PERIOD_CYCLES)

// Since we start at "off" cycle, the first change comes once the number of
// "off" cycles are over
#define BLU_COMPARE_CYCLES BLUE_OFF_CYCLES

// Timer and channel definitions for the Blue LED
#define BLU_TIMER TIMER_A0_BASE
#define BLU_CHANNEL TIMER_A_CAPTURECOMPARE_REGISTER_4

// Function declarations to be used in other files
void initPWMPins(void);
void startBluePWMTimer(void);
void stopBluePWMTimer(void);

#endif /* CODE_SOUNDS_H_ */
