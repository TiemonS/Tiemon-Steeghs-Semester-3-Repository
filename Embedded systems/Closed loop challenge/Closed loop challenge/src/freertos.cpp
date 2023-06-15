/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>
#include "semphr.h"

#include "UIManager.hpp"
#include "RobotAuto.hpp"
#include "DistanceSensor.hpp"
#include "PIDController.hpp"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart2;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
osThreadId_t UIThread;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal1,
    .tz_module = 0,
    .reserved = 0};

const osThreadAttr_t UITask_attributes = {
    .name = "uiTask",
    .attr_bits = osThreadDetached,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = NULL,
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal1,
    .tz_module = 0,
    .reserved = 0};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
PIDController pidController;
DistanceSensor distanceSensor;
ServoMotor servoMotorA;
ServoMotor servoMotorB;

RobotAuto* robotAuto;

UIManager uImanager;
int choice;
int tempChoice;

const int MSGBUFSIZE = 80;
char msgBuffer[MSGBUFSIZE];

//Semaphore
SemaphoreHandle_t g_mutex;

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void UIHandle(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  UIThread = osThreadNew(UIHandle, NULL, &UITask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

extern "C" void EXTI1_IRQHandler() {

    if (EXTI->PR & EXTI_PR_PR1) 
    {
        EXTI->PR = EXTI_PR_PR1;   // Clear interrupt flag for EXTI1 
        
        distanceSensor.InterruptHandler();
        //uImanager.printMessage("kees", &huart2);
    }
}

//De thread voor het beheren van de input afkomstig uit de UI, oftewel de robotauto
void StartDefaultTask(void *argument)
{

  robotAuto = new RobotAuto(pidController, distanceSensor, servoMotorA, servoMotorB);
  robotAuto->getServoMotorA().SetupMotorA();
  robotAuto->getServoMotorB().SetupMotorB();
  IDistanceSensor& robotDistanceSensor = robotAuto->getDistanceSensor();
  robotDistanceSensor.SetupTriggerTimer();
  robotDistanceSensor.SetupEchoTimer();
  robotDistanceSensor.SetupEchoInterrupt();
  /* Infinite loop */
  for (;;)
  {
    // UI-thread
     if (xSemaphoreTake(g_mutex, portMAX_DELAY) == pdTRUE) {
      // Kritieke sectie
      // Lees de gedeelde variabele uit
      robotAuto->HandleInput(choice);
      // Geef de mutex vrij
      xSemaphoreGive(g_mutex);

      // Voer andere taken uit
      // ...
    }

    osDelay(25);
  }
  /* USER CODE END StartDefaultTask */
}

void UIHandle(void *argument) 
{
  /* Create a mutex type semaphore. */
   g_mutex = xSemaphoreCreateMutex();

   if( g_mutex != NULL )
   {
    uImanager.printMessage("Mutex succesvol aangemaakt!", &huart2);

       /* The semaphore was created successfully and
       can be used. */
   }
   else 
   {
    /* The semaphore creation failed. Handle the error accordingly. */
    uImanager.printMessage("Mutex L", &huart2);
    }
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
  {
  // UI-thread
  tempChoice = uImanager.getUserInput(&huart2);
  uImanager.PrintUserInput(tempChoice, &huart2);
  if (xSemaphoreTake(g_mutex, portMAX_DELAY) == pdTRUE) {
    // Kritieke sectie
    choice = tempChoice;

    // Geef de mutex vrij
    xSemaphoreGive(g_mutex);

    // Voer andere taken uit
    // ...
  }
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}



/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

//TODO
//Fixen dat beiden threads goed werken zonder een print te gebruiken
//de verschillende methodes die ik heb testen hiervoor, sommige weghalen kijken waar het aan ligt
//osDelay vergroten of verkleinen misschien?
//print methode tijdelijk uitzetten dus alleen om informatie vragen
//kijken wat tim doet