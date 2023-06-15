#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "CarTimer.h"

void SetupTimer2Channel1() 
{

  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //Het aanzetten van de Timer 2 clock op het RCC_APB1ENR register (Pagina 154 in de datasheet)
  
  //Formule: f_timer = f_clock / (prescaler + 1) omgeschreven naar prescaler = (f_clock / f_timer) - 1
  //Het instellen van de prescaler timer van timer 2 op het TIMx_PSC register (Pagina 664)
  TIM2->PSC =  (SystemCoreClock / 1000000) - 1; 
  
  //De auto reload value is de waarde die bepaalt wanneer de update event wordt gegenereerd. Er wordt steeds getelt tot dat de count bereikt wordt.
  //TIMx_ARR Pagina 664 Formule: T = (PSC + 1) * (ARR + 1) / f_clock
  TIM2->ARR = 20000;

  // Het instellen van de timer2 capture/compare modus als PWM output TIMx_CCMR1 register (Pagina 656)
  // Hiervoor moeten bit 6 (OC1M2) and bit 5 (OC1M1) naar 1 en OC1M0 naar 0
  TIM2->CCMR1 = (TIM2->CCMR1 & ~TIM_CCMR1_OC1M_0) | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;

  //zet de preload aan om fouten te voorkomen, de nieuwe waardes die binnenkomen worden 
  //gebuffered en pas geupdate als de timer de volgende waarde bereikt heeft
  //TIM2->CCMR1 |= TIM_CCMR1_OC1PE; 

  TIM2->CCER |= TIM_CCER_CC1E; // het aanzetten van de pwm capture/compare 1 output (Pagine 662)

  //zorgt ervoor dat de polariteit van de uitvoer van het kanaal niet wordt omgekeerd
  TIM2->CCER &= ~(TIM_CCER_CC1P_Msk | TIM_CCER_CC1NP_Msk);

  // Enable Timer 2 counter
  TIM2->CR1 |= TIM_CR1_CEN;

  //TIM2->CCR1 = 1700; //De servo op kanaal 1 van timer 2 naar neutral zetten
  
  GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODER0) | (0b10 << GPIO_MODER_MODER0_Pos); //Pib PA0 (Servo) instellen als alternatieve functie
  GPIOA->AFR[0] = (GPIOA->AFR[0] & ~GPIO_AFRL_AFRL0) | (0b0001 << GPIO_AFRL_AFRL0_Pos); //de correcte alternate function instellen (AFR 1) GPIOx_AFRL register pagina 241
}

void SetupTimer2Channel2() 
{

  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //Het aanzetten van de Timer 2 clock op het RCC_APB1ENR register (Pagina 154 in de datasheet)
  
  //Formule: f_timer = f_clock / (prescaler + 1) omgeschreven naar prescaler = (f_clock / f_timer) - 1
  //Het instellen van de prescaler timer van timer 2 op het TIMx_PSC register (Pagina 664)
  TIM2->PSC =  (SystemCoreClock / 1000000) - 1; 
  
  //De auto reload value is de waarde die bepaalt wanneer de update event wordt gegenereerd. Er wordt steeds getelt tot dat de count bereikt wordt.
  //TIMx_ARR Pagina 664 Formule: T = (PSC + 1) * (ARR + 1) / f_clock
  TIM2->ARR = 20000;

  // Het instellen van de timer2 capture/compare modus als PWM output TIMx_CCMR1 register (Pagina 656)
  // Hiervoor moeten bit 6 (OC1M2) and bit 5 (OC1M1) naar 1 en OC1M0 naar 0
  TIM2->CCMR1 = (TIM2->CCMR1 & ~TIM_CCMR1_OC2M_0) | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

  //zet de preload aan om fouten te voorkomen, de nieuwe waardes die binnenkomen worden 
  //gebuffered en pas geupdate als de timer de volgende waarde bereikt heeft
  //TIM2->CCMR1 |= TIM_CCMR1_OC1PE; 

  TIM2->CCER |= TIM_CCER_CC2E; // het aanzetten van de pwm capture/compare 1 output (Pagine 662)

  //zorgt ervoor dat de polariteit van de uitvoer van het kanaal niet wordt omgekeerd
  TIM2->CCER &= ~(TIM_CCER_CC2P_Msk | TIM_CCER_CC2NP_Msk);

  // Enable Timer 2 counter
  TIM2->CR1 |= TIM_CR1_CEN;

  //TIM2->CCR1 = 1700; //De servo op kanaal 1 van timer 2 naar neutral zetten
  
  GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODER1) | (0b10 << GPIO_MODER_MODER1_Pos); //Pib PA1 (Servo2) instellen als alternatieve functie
  GPIOA->AFR[0] = (GPIOA->AFR[0] & ~GPIO_AFRL_AFRL1) | (0b0001 << GPIO_AFRL_AFRL1_Pos); //de correcte alternate function instellen (AFR 1) GPIOx_AFRL register pagina 241
  
}

void SetupTimer3Channel3() 
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

  TIM3->PSC =  (SystemCoreClock / 1000000) - 1; 

  TIM3->ARR = 25000; //Omgerekend 25 ms dat de tijd tussen twee pulsen duurt

  TIM3->CCMR2 = (TIM2->CCMR2 & ~TIM_CCMR2_OC3M_0) | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;

  TIM3->CCER |= TIM_CCER_CC3E;

  TIM3->CCER &= ~(TIM_CCER_CC3P_Msk | TIM_CCER_CC3NP_Msk);

  TIM3->CR1 |= TIM_CR1_CEN;

  TIM3->CCR3 = 10; //De tijd dat het signaal duurt (10 micro seconden)
  
  GPIOB->MODER = (GPIOA->MODER & ~GPIO_MODER_MODER0) | (0b10 << GPIO_MODER_MODER0_Pos); //Pin PB0 (Sensor Trigger) instellen als alternatieve functie
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFRL0) | (0b0010 << GPIO_AFRL_AFRL0_Pos);

}

void SetupTimer4Channel1()
{
  //TODO
  //voor de echo gebruik ik timer 4 channel 1 op pin PB6 (D10)
  //ik moet ook de interrupt van de timer gebruiken (geen normale interrupt)
  //met deze timer moet ik de afstand berekenen

  // Enable the Timer 4 clock
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

  // Set the prescaler to scale down the timer clock to 1 MHz
  TIM4->PSC = (SystemCoreClock / 1000000) - 1; 

  // Set CC1 channel as input with IC1 mapped on TI1
  TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;

  // Set CC1NP/CC1P bits to non-inverted/rising edge
  TIM4->CCER &= ~(TIM_CCER_CC1NP | TIM_CCER_CC1P);

  // Enable Capture/Compare 1 capture
  TIM4->CCER |= TIM_CCER_CC1E;

  // Set CC2 channel as input with IC2 mapped on TI1
  TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;

  // Set CC2NP/CC2P bits to inverted/falling edge
  TIM4->CCER &= ~TIM_CCER_CC2NP;
  TIM4->CCER |= TIM_CCER_CC2P;

  // Enable Capture/Compare 2 capture
  TIM4->CCER |= TIM_CCER_CC2E;

  // Set Trigger selection to Filtered Timer Input 1
  TIM4->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0;

  // Set Slave mode selection to Reset Mode
  TIM4->SMCR |= TIM_SMCR_SMS_2;

  // Enable the counter
  TIM4->CR1 |= TIM_CR1_CEN;

  // Configure PB6 as alternate function mode
  GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER6) | (0b10 << GPIO_MODER_MODER6_Pos);

  // Set PB6 as alternate function AF2 (TIM4_CH1)
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFRL6) | (0b0010 << GPIO_AFRL_AFRL6_Pos);
}

int readTimer4Value()
{
  return TIM4->CCR1;
}