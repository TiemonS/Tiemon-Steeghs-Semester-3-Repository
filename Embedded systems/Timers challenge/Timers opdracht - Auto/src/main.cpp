/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#define PWM_PERIOD_US 20000  // PWM period in microseconds
#define PWM_MIN_US 1000  // PWM minimum pulse width in microseconds
#define PWM_MAX_US 2000  // PWM maximum pulse width in microseconds
#define PWM_NEUTRAL_US 1500  // PWM neutral pulse width in microseconds

void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

unsigned long RisingEdgeMillisTime = 0;
unsigned long ButtonPressedTime = 0;
unsigned long previousMillis = 0;

const int MSGBUFSIZE = 80;
char msgBuf[MSGBUFSIZE];

bool ShortPress = false;

void SetupTimer() 
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

void LedModi(int Modi, int Speed) 
{
  switch (Modi)
  {
  case 1:
  if (HAL_GetTick() >= previousMillis + Speed)
  {
    GPIOB->ODR ^= GPIO_ODR_5;
    previousMillis = HAL_GetTick();
  }
    break;
  case 2:
    GPIOB->ODR |= GPIO_ODR_5; //Het togglen van de LED op pin PB5
    break;
  }
}

extern "C" void EXTI0_IRQHandler(void)  
{
    EXTI->PR = EXTI_PR_PR0;   // Interrupt flag resetten zodat ook de volgende intterupt goed verwerkt wordt
    //GPIOB->ODR ^= GPIO_ODR_5; //Het togglen van de LED op pin PB5
  
    snprintf(msgBuf, MSGBUFSIZE, "%s", "Interrupt aan!\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

    //is de knop ingedrukt?
    if ((GPIOC->IDR & GPIO_IDR_0) != 0)
    {
    //sla de risingedge tijd op
    RisingEdgeMillisTime = HAL_GetTick();
    }
    //is de knop niet meer ingedrukt? Dus falling edge getriggered
    else
    {
    //bereken het tijdsverschil tussen rising en falling edge
    ButtonPressedTime = HAL_GetTick() - RisingEdgeMillisTime;
    }
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();




  //de methode waarin timer2 wordt ingesteld op kanaal 1
  SetupTimer();

  //als eerste moet ik de PA0 PIN waarop ik de led heb aangelosten instellen op output
  //Zie GPIO port mode register 
  //ob00 INPUT
  //0b01 OUTPUT 
  GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER5) | (0b01 << GPIO_MODER_MODER5_Pos); //Pin PB5 (De led) Instellen als OUTPUT pin
  GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODER0) | (0b00 << GPIO_MODER_MODER0_Pos); //Pin P0C (De knop) instellen als INPUT pin
  
 // GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODER0) | (0b10 << GPIO_MODER_MODER0_Pos); //Pib PA0 (Servo) instellen als alternatieve functie
 // GPIOA->AFR[0] = (GPIOA->AFR[0] & ~GPIO_AFRL_AFRL2) | (0b0001 << GPIO_AFRL_AFRL2_Pos); //register Pagina 241

  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_0;

  //dan vervolgens zet ik PINC0 in als interrupt pin dit doe ik in het SYSCFG_EXTICR0 register 
  SYSCFG->EXTICR[0] = (SYSCFG->EXTICR[0] & ~SYSCFG_EXTICR1_EXTI0) | (0b010 << SYSCFG_EXTICR1_EXTI0_Pos); // pin PC0 to interrupt EXTI0


  EXTI->FTSR = EXTI_FTSR_TR0;   // Set interrupt EXTI0 trigger to falling edge
  EXTI->RTSR = EXTI_RTSR_TR0;   // ook nog rising edge aanzetten
  EXTI->IMR = EXTI_IMR_MR0; //EXTI0 Unmasken     
  NVIC_EnableIRQ(EXTI0_IRQn); 


  while (1)
  {
    GPIOA->ODR |= GPIO_ODR_0;  
    if (ButtonPressedTime > 1000 && ButtonPressedTime < 2000)
    {
      LedModi(1, 500);
      snprintf(msgBuf, MSGBUFSIZE, "%s", "aiergjaierugjaerug\r\n");
      HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
    }
    else if (ButtonPressedTime > 30 && ButtonPressedTime < 1000)
    {
      LedModi(2, 500);
      snprintf(msgBuf, MSGBUFSIZE, "%s", "zxcvzxcvczxv4578suightrsg\r\n");
      HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
    }

    for (int i = 0; i <= 180; i++) {
            TIM2->CCR1 = 1000 + (i * 11);
            for (int j = 0; j < 500000; j++); // delay
        }
    for (int i = 180; i >= 0; i--) {
        TIM2->CCR1 = 1000 + (i * 11);
        for (int j = 0; j < 500000; j++); // delay
    }
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM17)
  {
    HAL_IncTick();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif