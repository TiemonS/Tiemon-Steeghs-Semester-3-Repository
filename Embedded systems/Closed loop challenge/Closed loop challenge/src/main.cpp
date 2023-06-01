/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "CarTimer.h"
#include "PIDController.hpp"
#include "ServoMotor.hpp"
#include "DistanceSensor.hpp"
#include "UIManager.hpp"

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
bool servosEnabled = false;

unsigned long RisingEdgeTime = 0;
unsigned long PulsTravelTime = 0;

PIDController pidController;
UIManager uImanager;

void LedModi(int Modi, int Speed) 
{
  switch (Modi)
  {
  case 1:
  if (HAL_GetTick() >= previousMillis + Speed)
  {
    GPIOB->ODR ^= GPIO_ODR_4;
    previousMillis = HAL_GetTick();
  }
    break;
  case 2:
    GPIOB->ODR |= GPIO_ODR_4; //Het togglen van de LED op pin PB5
    break;
  }
}

extern "C" void EXTI0_IRQHandler(void)  
{
    EXTI->PR = EXTI_PR_PR0;   // Interrupt flag resetten zodat ook de volgende intterupt goed verwerkt wordt
    //GPIOB->ODR ^= GPIO_ODR_5; //Het togglen van de LED op pin PB5
  
    //snprintf(msgBuf, MSGBUFSIZE, "%s", "Interrupt aan!\r\n");
    //HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

  	
    //is de knop ingedrukt?
    if ((GPIOC->IDR & GPIO_IDR_0) != 0)
    {
    //sla de risingedge tijd op
    RisingEdgeMillisTime = HAL_GetTick();
    servosEnabled = !servosEnabled;
    }
    //is de knop niet meer ingedrukt? Dus falling edge getriggered
    else
    {
    //bereken het tijdsverschil tussen rising en falling edge
    ButtonPressedTime = HAL_GetTick() - RisingEdgeMillisTime;
    }
}

//De interrupt handle methode voor pin PC0 (SensorEchoPin)
extern "C" void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR1) {
        EXTI->PR = EXTI_PR_PR1;   // Clear interrupt flag for EXTI1

        //voor debuggen
        // snprintf(msgBuf, MSGBUFSIZE, "%s", "Interrupt triggered on PC1!\r\n");
        // HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

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
}

void sendUARTMessage(UART_HandleTypeDef *huart, const char *message) {
    char msgBuf[MSGBUFSIZE];
    snprintf(msgBuf, MSGBUFSIZE, "%s", message);
    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
  }

int main(void)
{
  //initialiseer methodes van het stm32 bord
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  //methodes voor het opzetten van de timers
  SetupTimer2Channel1(); //De pwm output timer van servo 1
  SetupTimer2Channel2(); //De pwm output timer van servo 2
  SetupTimer3Channel3(); //De pwm output timer voor de sensor trigger
  
  //De pwm input timer, Deze werkt momenteel niet goed waardoor 
  //ik alleen de count van deze timer gebruik voor de echo pin van de sensor
  SetupTimer4Channel1(); 

  pidController.SetSetpoint(10);


  //als eerste moet ik de PA0 PIN waarop ik de led heb aangelosten instellen op output
  //Zie GPIO port mode register 
  //ob00 INPUT
  //0b01 OUTPUT 
  GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER4) | (0b01 << GPIO_MODER_MODER4_Pos); //Pin PB4 (De led) Instellen als OUTPUT pin
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

  ////////////////////////////
  ////////////////////////////
  ///Interrupt van de echo
  ///////////////////////////

  // Configure PC1 as input
  GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODER1) | (0b00 << GPIO_MODER_MODER1_Pos);

  // Configure PC1 as interrupt pin
  SYSCFG->EXTICR[0] = (SYSCFG->EXTICR[0] & ~SYSCFG_EXTICR1_EXTI1) | (0b010 << SYSCFG_EXTICR1_EXTI1_Pos); // pin PC1 to interrupt EXTI1

  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_1; // Set output type to push-pull for PC1

  EXTI->FTSR |= EXTI_FTSR_TR1;   // Set interrupt EXTI1 trigger to falling edge
  EXTI->RTSR |= EXTI_RTSR_TR1;   // Set interrupt EXTI1 trigger to rising edge
  EXTI->IMR |= EXTI_IMR_MR1;     // EXTI1 Unmask
  NVIC_EnableIRQ(EXTI1_IRQn);    // Enable the EXTI1 interrupt in the NVIC

  // ES Course Comments: Uncomment the three lines below to enable FreeRTOS.
  osKernelInitialize(); /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  osKernelStart(); /* Start scheduler */



  while (1)
  {
    GPIOA->ODR |= GPIO_ODR_0;  
    if (ButtonPressedTime > 1000 && ButtonPressedTime < 2000)
    {
      LedModi(1, 500);
      //snprintf(msgBuf, MSGBUFSIZE, "%s", "aiergjaierugjaerug\r\n");
      //HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
    }
    else if (ButtonPressedTime > 30 && ButtonPressedTime < 1000)
    {
      LedModi(2, 500);
      //snprintf(msgBuf, MSGBUFSIZE, "%s", "zxcvzxcvczxv4578suightrsg\r\n");
      //HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
    }

    int distance = PulsTravelTime * 0.0343 / 2; //het berekenen van de afstand in cm (Tijd in uS * snelheid van geluid in uS/cm / 2 omdat het heen en weer gaat)

    // snprintf(msgBuf, MSGBUFSIZE, "%d", distance);
    // HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
  
    // snprintf(msgBuf, MSGBUFSIZE, "%s", " cm\r\n");
    // HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

    pidController.SetInput(double(distance));

    int motorMinSpeed = 1280; // Minimum motor speed
    int motorMaxSpeed = 1720; // Maximum motor speed
    double inputMin = 1.0; // Minimum input distance (cm)
    double inputMax = 400.0; // Maximum input distance (cm)

    pidController.Compute();

    //int mapOutput = pidController.mapOutput(pidController.GetOutput(), motorMinSpeed, motorMaxSpeed, inputMin, inputMax);
    int mapOutput = 1500 + pidController.GetOutput();
    int mapOutput2 = 1500 - pidController.GetOutput();
    // snprintf(msgBuf, MSGBUFSIZE, "%d", mapOutput);
    // HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

    // snprintf(msgBuf, MSGBUFSIZE, "%s", " Servo Val\r\n");
    // HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

    if (servosEnabled)
    {
      TIM2->CCR1 = mapOutput;
      TIM2->CCR2 = mapOutput2;
    }

    // 1280 - 1480 Clockwise
    // 1480 - 1520 Stop
    // 1520 - 1720 Counter-clockwise
    // TIM2->CCR1 = 1600; 
    // TIM2->CCR2 = 1400; 
    
    HAL_Delay(25);
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

/////////TODO///////////////
/*
- Afstandmeting afwijking fixen (Die wordt veroorzaakt dat de count reset om de zoveel tijd, 
dus door te controlleren of de begintijd niet groter is dan de eindtijd want dit zou niet moeten kunnen)

- Abstractielaag verder afmaken

- Code schoonmaken
*/