#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "DistanceSensor.hpp"
#include "CarTimer.h"

DistanceSensor::DistanceSensor() 
{

}

DistanceSensor::~DistanceSensor() {}

//het berekenen van de afstand in cm (Tijd in uS * snelheid van geluid in uS/cm / 2 omdat het heen en weer gaat)
int DistanceSensor::CalculateDistance() 
{
    int distance = PulsTravelTime * 0.0343 / 2; 
    return distance;
}

void DistanceSensor::SetupEchoInterrupt() 
{
    // Configure PC1 as input
  GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODER1) | (0b00 << GPIO_MODER_MODER1_Pos);

  // Configure PC1 as interrupt pin
  SYSCFG->EXTICR[0] = (SYSCFG->EXTICR[0] & ~SYSCFG_EXTICR1_EXTI1) | (0b010 << SYSCFG_EXTICR1_EXTI1_Pos); // pin PC1 to interrupt EXTI1

  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_1; // Set output type to push-pull for PC1

  EXTI->FTSR |= EXTI_FTSR_TR1;   // Set interrupt EXTI1 trigger to falling edge
  EXTI->RTSR |= EXTI_RTSR_TR1;   // Set interrupt EXTI1 trigger to rising edge
  EXTI->IMR |= EXTI_IMR_MR1;     // EXTI1 Unmask
  NVIC_EnableIRQ(EXTI1_IRQn);    // Enable the EXTI1 interrupt in the NVIC
}

void DistanceSensor::InterruptHandler() 
{
    //Is de echo pin hoog?
    if ((GPIOC->IDR & GPIO_IDR_1) != 0)
    {
    //sla de risingedge tijd op
    RisingEdgeTime = TIM4->CNT;
    }
    //is de pin niet meer hoog? Dus falling edge getriggered
    //en nog even kijken of de timer niet net gereset is
    else if(TIM4->CNT > RisingEdgeTime)
    {
    //bereken het tijdsverschil tussen rising en falling edge
    PulsTravelTime = TIM4->CNT - RisingEdgeTime;
    }
}

void DistanceSensor::SetupTriggerTimer() 
{
    SetupTimer3Channel3(); //De pwm output timer voor de sensor trigger
}

void DistanceSensor::SetupEchoTimer() 
{
    //De pwm input timer, Deze werkt momenteel niet goed waardoor 
    //ik alleen de count van deze timer gebruik voor de echo pin van de sensor
    SetupTimer4Channel1(); 
}

unsigned long DistanceSensor::GetRisingEdgeTime() const {
    return RisingEdgeTime;
}

