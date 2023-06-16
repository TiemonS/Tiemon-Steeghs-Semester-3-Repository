#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "UIManager.hpp"

int UIManager::getUserInput(UART_HandleTypeDef *huart) {
    char msgBuf[msgBufSize];
    snprintf(msgBuf, msgBufSize, "==================\nMenu:\n1. Servos aan\n2. Servos uit\n3. PID aanzetten\n\nVoer uw keuze in: ");
    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

    char userInput[input_Size];
    HAL_UART_Receive(huart, (uint8_t*)userInput, input_Size, HAL_MAX_DELAY);

    int choice = atoi(userInput);
    return choice;
}

void UIManager::PrintUserInput(int choice, UART_HandleTypeDef *huart) {
    char msgBuf[msgBufSize];
    const char* message;
    
    switch (choice) {
        case 1:
            // Optie 1 geselecteerd
            message = "\nServos gaan aan!\n";
            break;
        case 2:
            // Optie 2 geselecteerd
            message = "\nServos gaan uit!\n";
            break;
        case 3:
            // Optie 3 geselecteerd
            message = "\nPID aangezet!.\n";
            break;
        default:
            message = "\nOngeldige keuze.\n";
            break;
    }
    snprintf(msgBuf, msgBufSize, message);
    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);      
}

void UIManager::printMessage(const char* message, UART_HandleTypeDef *huart)
{
    char msgBuf[msgBufSize];
    snprintf(msgBuf, msgBufSize, "%s\r\n", message);
    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
}

void UIManager::printDistance(int distance, UART_HandleTypeDef *huart) 
{
    char msgBuf[msgBufSize];
    snprintf(msgBuf, msgBufSize, "%d", distance);
    HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

    snprintf(msgBuf, msgBufSize, "%s", " cm\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
}