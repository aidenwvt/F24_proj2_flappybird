/*
 * sounds.c
 *
 *  Created on: Oct 22, 2024
 *      Author: Aiden Wiehn
 */

#include "Code/Header/sounds.h"

// I ran this buzzer at 2am and woke my roommate up
// "bwahhhhhhhhh" - the buzzer

void initPWMPins() {
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
}

void startBluePWMTimer(void) {
    Timer_A_PWMConfig pwmConfig_blu;
    pwmConfig_blu.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    pwmConfig_blu.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
    pwmConfig_blu.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET;
    pwmConfig_blu.compareRegister = BLU_CHANNEL;
    pwmConfig_blu.timerPeriod = PWM_PERIOD_CYCLES;
    pwmConfig_blu.dutyCycle = BLU_COMPARE_CYCLES;

    initPWMPins();
    Timer_A_generatePWM(BLU_TIMER, &pwmConfig_blu);
}

void stopBluePWMTimer(void) {
    Timer_A_stopTimer(BLU_TIMER);
}
