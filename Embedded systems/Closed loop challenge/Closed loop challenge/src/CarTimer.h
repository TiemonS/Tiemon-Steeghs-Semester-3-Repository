#ifndef CARTIMER_H
#define CARTIMER_H

void SetupTimer();
void SetupTimer2(); 
void SetupTriggerTimer();
int readTimer4Value();
void setupPWMInput();
void timer4_channel1_setup();

void SetupEchoInterrupt();
extern "C" void EXTI1_IRQHandler(void);

#endif