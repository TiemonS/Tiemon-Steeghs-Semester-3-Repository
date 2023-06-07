#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "UIManager.hpp"

int UIManager::getUserInput(UART_HandleTypeDef *huart) {
    char msgBuf[msgBufSize];
    snprintf(msgBuf, msgBufSize, "==================\nMenu:\n1. Servos aan\n2. Servos uit\n3. Optie 3\n4. Afsluiten\n\nVoer uw keuze in: ");
    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

    char userInput[input_Size];
    HAL_UART_Receive(huart, (uint8_t*)userInput, input_Size, HAL_MAX_DELAY);

    int choice = atoi(userInput);
    recievedMessage = true;
    return choice;
}

void UIManager::handleUserInput(int choice, UART_HandleTypeDef *huart) {
    char msgBuf[msgBufSize];
    
    if (recievedMessage)
    {
        switch (choice) {
            case 1:
                // Optie 1 geselecteerd
                snprintf(msgBuf, msgBufSize, "\nServos gaan aan!\n\n");
                break;
            case 2:
                // Optie 2 geselecteerd
                snprintf(msgBuf, msgBufSize, "\nServos gaan uit!\n\n");
                break;
            case 3:
                // Optie 3 geselecteerd
                snprintf(msgBuf, msgBufSize, "\nOptie 3 geselecteerd.\n\n");
                break;
            case 4:
                // Afsluiten geselecteerd
                snprintf(msgBuf, msgBufSize, "\nProgramma wordt afgesloten.\n\n");
                // Voer hier code uit om het programma af te sluiten
                break;
            default:
                snprintf(msgBuf, msgBufSize, "Ongeldige keuze.\r\n");
                break;
        }
        HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
        recievedMessage = false;
   }   

}

void UIManager::printMessage(const char* message, UART_HandleTypeDef *huart)
{
    char msgBuf[msgBufSize];
    snprintf(msgBuf, msgBufSize, "%s\r\n", message);
    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
}